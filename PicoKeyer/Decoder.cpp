// 
// 
// 

#include "Decoder.h"
#include "keyer_pin_settings.h"


#ifdef ARDUINO_RASPBERRY_PI_PICO

// CW Decoder - runs on the second core..


float magnitude;
int magnitudelimit = 600;
int magnitudelimit_low = 300;
int realstate = LOW;
int realstatebefore = LOW;
int filteredstate = LOW;
int filteredstatebefore = LOW;


///////////////////////////////////////////////////////////
// The sampling frq will be 8928 on a 16 mhz             //
// without any prescaler etc                             //
// because we need the tone in the center of the bins    //
// you can set the tone to 496, 558, 744 or 992          //
// then bucket_count the number of samples which give the bandwidth //
// can be (8928 / tone) * 1 or 2 or 3 or 4 etc           //
// init is 8928/558 = 16 *4 = 64 samples                 //
// try to take bucket_count = 96 or 128 ;o)                         //
// 48 will give you a bandwidth around 186 hz            //
// 64 will give you a bandwidth around 140 hz            //
// 96 will give you a bandwidth around 94 hz             //
// 128 will give you a bandwidth around 70 hz            //
// BUT remember that high bucket_count take a lot of time           //
// so you have to find the compromice - i use 48         //
///////////////////////////////////////////////////////////

/*

The above comment refers to early Arduino boards which were relatively slow. Also, they were single core and the decoder code had
to share that core with all the other keyer functions. The original code here sampled in a tight loop and the sampling frequency
was the maximum posisble on the processor in question. 

The RP2040 processor in the Pi Pico is much faster and dual core - this changes the algorithm somewhat. Here, we specify the sampling
frequency in samples per second and, from that, calculate the delay in microseconds between samples. In the running loop, we read
samples into the testData array with a delay of the required length between reads in order to achieve the requires sample rate.
Since the second core is dedicated to the CW decoder, we can afford to use the kernel delay function which puts the core to sleep.

Note also that the testData array is of a fixed size and dimensoned large. The RP2040 processor has a lot more RAM than any of
the original Arduinos and there is no need to restrict the size of the data array. Dimensioning it large allows us to dynamically
reconfigure the decoder at run time.

*/

float coeff;
float Q1 = 0;
float Q2 = 0;
float sine;
float cosine;
float sampling_freq = 17856; // 8928; // 53568; // 74214.0; // 8928.0;
float target_freq = 750; // 558; // 625.0; // 558.0; /// adjust for your needs see above
float bucket_count = 96; // 133.0; //48.0;  //// if you change  her please change next line also 
float d = ((1e6 / sampling_freq) - 2)/1.1;  // inter-sample delay - effectively the reciprical of the sampling frequency - with a small fiddle-factor to allow for the processing time in the decode loop
int testData[800]; // must be dimensioned to larger than the maximum permitted bucket_count

volatile bool decode = false;

//////////////////////////////
// Noise Blanker time which //
// shall be computed so     //
// this is initial          //
//////////////////////////////
int nbtime = 6;  /// ms noise blanker         

long starttimehigh;
long highduration;
long lasthighduration;
long hightimesavg;
long lowtimesavg;
long startttimelow;
long lowduration;
long laststarttime = 0;

char code[20];
int stop = LOW;
int wpm;

/////////////////////////////////////
// print the ascii code to the lcd //
// one a time so we can generate   //
// special letters                 //
/////////////////////////////////////
void printascii(int asciinumber) {

    // Serial.print(char(asciinumber));
    rp2040.fifo.push_nb(asciinumber);

}


////////////////////////////////
// translate cw code to ascii //
////////////////////////////////

void docode() {
    if (strcmp(code, ".-") == 0) printascii(65);
    if (strcmp(code, "-...") == 0) printascii(66);
    if (strcmp(code, "-.-.") == 0) printascii(67);
    if (strcmp(code, "-..") == 0) printascii(68);
    if (strcmp(code, ".") == 0) printascii(69);
    if (strcmp(code, "..-.") == 0) printascii(70);
    if (strcmp(code, "--.") == 0) printascii(71);
    if (strcmp(code, "....") == 0) printascii(72);
    if (strcmp(code, "..") == 0) printascii(73);
    if (strcmp(code, ".---") == 0) printascii(74);
    if (strcmp(code, "-.-") == 0) printascii(75);
    if (strcmp(code, ".-..") == 0) printascii(76);
    if (strcmp(code, "--") == 0) printascii(77);
    if (strcmp(code, "-.") == 0) printascii(78);
    if (strcmp(code, "---") == 0) printascii(79);
    if (strcmp(code, ".--.") == 0) printascii(80);
    if (strcmp(code, "--.-") == 0) printascii(81);
    if (strcmp(code, ".-.") == 0) printascii(82);
    if (strcmp(code, "...") == 0) printascii(83);
    if (strcmp(code, "-") == 0) printascii(84);
    if (strcmp(code, "..-") == 0) printascii(85);
    if (strcmp(code, "...-") == 0) printascii(86);
    if (strcmp(code, ".--") == 0) printascii(87);
    if (strcmp(code, "-..-") == 0) printascii(88);
    if (strcmp(code, "-.--") == 0) printascii(89);
    if (strcmp(code, "--..") == 0) printascii(90);

    if (strcmp(code, ".----") == 0) printascii(49);
    if (strcmp(code, "..---") == 0) printascii(50);
    if (strcmp(code, "...--") == 0) printascii(51);
    if (strcmp(code, "....-") == 0) printascii(52);
    if (strcmp(code, ".....") == 0) printascii(53);
    if (strcmp(code, "-....") == 0) printascii(54);
    if (strcmp(code, "--...") == 0) printascii(55);
    if (strcmp(code, "---..") == 0) printascii(56);
    if (strcmp(code, "----.") == 0) printascii(57);
    if (strcmp(code, "-----") == 0) printascii(48);

    if (strcmp(code, "..--..") == 0) printascii(63);
    if (strcmp(code, ".-.-.-") == 0) printascii(46);
    if (strcmp(code, "--..--") == 0) printascii(44);
    if (strcmp(code, "-.-.--") == 0) printascii(33);
    if (strcmp(code, ".--.-.") == 0) printascii(64);
    if (strcmp(code, "---...") == 0) printascii(58);
    if (strcmp(code, "-....-") == 0) printascii(45);
    if (strcmp(code, "-..-.") == 0) printascii(47);

    if (strcmp(code, "-.--.") == 0) printascii(40);
    if (strcmp(code, "-.--.-") == 0) printascii(41);
    if (strcmp(code, ".-...") == 0) printascii(95);
    if (strcmp(code, "...-..-") == 0) printascii(36);
    if (strcmp(code, "...-.-") == 0) printascii(62);
    if (strcmp(code, ".-.-.") == 0) printascii(60);
    if (strcmp(code, "...-.") == 0) printascii(126);
    //////////////////
    // The specials //
    //////////////////
    if (strcmp(code, ".-.-") == 0) printascii(3);
    if (strcmp(code, "---.") == 0) printascii(4);
    if (strcmp(code, ".--.-") == 0) printascii(6);

}


//void updateinfolinelcd() {
//    /////////////////////////////////////
//    // here we update the upper line   //
//    // with the speed.                 //
//    /////////////////////////////////////
//
//     // int place;
//     // if (rows == 4){
//     //  place = colums/2;}
//     // else{
//     //  place = 2;
//     // }
//        //if (wpm<10){
//        //	lcd.setCursor((place)-2,0);
//        //	lcd.print("0");
//        //	lcd.setCursor((place)-1,0);
//        //	lcd.print(wpm);
//        //	lcd.setCursor((place),0);
//        //	lcd.print(" WPM");
//        //}
//        //else{
//        //	lcd.setCursor((place)-2,0);
//        //	lcd.print(wpm);
//        //	lcd.setCursor((place),0);
//        //	lcd.print(" WPM ");
//        //}
//
//}

void calculate_decoder_parameters() {
    Serial.printf("Recalculating decoder parameters %f, %f, %f\n", sampling_freq, target_freq, bucket_count);
    decode = false;     // disable decoding while parameters are updated
    ////////////////////////////////////
    // The basic goertzel calculation //
    ////////////////////////////////////
    int	k;
    float	omega;
    k = (int)(0.5 + ((bucket_count * target_freq) / sampling_freq));
    omega = (2.0 * PI * k) / bucket_count;
    sine = sin(omega);
    cosine = cos(omega);
    coeff = 2.0 * cosine;
    d = ((1e6 / sampling_freq) - 2) / 1.1;
    Serial.println("Decoder parameters recalculated");
    decode = true;      // resume decoding
}

////////////////
// init setup //
////////////////
void setup1() {
    calculate_decoder_parameters();

    pinMode(cw_decoder_indicator, OUTPUT); // probably not required here
}


//int i = 0;

void loop1() {

    ///////////////////////////////////// 
    // The basic where we get the tone //
    /////////////////////////////////////

    if (!decode) return;    // configuration in progress...

    unsigned long stime = micros();

    for (char index = 0; index < bucket_count; index++)
    {
        testData[index] = analogRead(cw_decoder_audio_input_pin);
        if (index < bucket_count) delayMicroseconds(trunc(d));
    }

    unsigned long etime = micros();
    //Serial.printf("Stime is %lu and etime is %lu\n", stime, etime);
    //Serial.printf("Sample rate is %f\n", n/ ((etime - stime)/1000000.0));

    for (char index = 0; index < bucket_count; index++) {
        float Q0;
        Q0 = coeff * Q1 - Q2 + (float)testData[index];
        Q2 = Q1;
        Q1 = Q0;
    }
    float magnitudeSquared = (Q1 * Q1) + (Q2 * Q2) - Q1 * Q2 * coeff;  // we do only need the real part //
    magnitude = sqrt(magnitudeSquared);
    Q2 = 0;
    Q1 = 0;

    //Serial.print(magnitude); Serial.println();  //// here you can measure magnitude for setup..

    /////////////////////////////////////////////////////////// 
    // here we will try to set the magnitude limit automatic //
    ///////////////////////////////////////////////////////////

    if (magnitude > magnitudelimit_low) {
        magnitudelimit = (magnitudelimit + ((magnitude - magnitudelimit) / 6));  /// moving average filter
    }

    if (magnitudelimit < magnitudelimit_low)
        magnitudelimit = magnitudelimit_low;

    ////////////////////////////////////
    // now we check for the magnitude //
    ////////////////////////////////////

    if (magnitude > magnitudelimit * 0.6) // just to have some space up 
        realstate = HIGH;
    else
        realstate = LOW;

    ///////////////////////////////////////////////////// 
    // here we clean up the state with a noise blanker //
    /////////////////////////////////////////////////////

    if (realstate != realstatebefore) {
        laststarttime = millis();
    }
    if ((millis() - laststarttime) > nbtime) {
        if (realstate != filteredstate) {
            filteredstate = realstate;
        }
    }

    ////////////////////////////////////////////////////////////
    // Then we do want to have some durations on high and low //
    ////////////////////////////////////////////////////////////

    if (filteredstate != filteredstatebefore) {
        if (filteredstate == HIGH) {
            starttimehigh = millis();
            lowduration = (millis() - startttimelow);
        }

        if (filteredstate == LOW) {
            startttimelow = millis();
            highduration = (millis() - starttimehigh);
            if (highduration < (2 * hightimesavg) || hightimesavg == 0) {
                hightimesavg = (highduration + hightimesavg + hightimesavg) / 3;     // now we know avg dit time ( rolling 3 avg)
            }
            if (highduration > (5 * hightimesavg)) {
                hightimesavg = highduration + hightimesavg;     // if speed decrease fast ..
            }
        }
    }


    ///////////////////////////////////////////////////////////////
    // now we will check which kind of baud we have - dit or dah //
    // and what kind of pause we do have 1 - 3 or 7 pause        //
    // we think that hightimeavg = 1 bit                         //
    ///////////////////////////////////////////////////////////////

    if (filteredstate != filteredstatebefore) {
        stop = LOW;
        if (filteredstate == LOW) {  //// we did end a HIGH
            if (highduration < (hightimesavg * 2) && highduration >(hightimesavg * 0.6)) { /// 0.6 filter out false dits
                strcat(code, ".");
            //    Serial2.print(".");
            }
            if (highduration > (hightimesavg * 2) && highduration < (hightimesavg * 6)) {
                strcat(code, "-");
            //    Serial2.print("-");
                wpm = (wpm + (1200 / ((highduration) / 3))) / 2;  //// the most precise we can do ;o)
            }
        }

        if (filteredstate == HIGH) {  //// we did end a LOW

            float lacktime = 1;
            if (wpm > 25)lacktime = 1.0; ///  when high speeds we have to have a little more pause before new letter or new word 
            if (wpm > 30)lacktime = 1.2;
            if (wpm > 35)lacktime = 1.5;

            if (lowduration > (hightimesavg * (2 * lacktime)) && lowduration < hightimesavg * (5 * lacktime)) { // letter space
                docode();
                code[0] = '\0';
           //     Serial2.print("/");
            }
            if (lowduration >= hightimesavg * (5 * lacktime)) { // word space
                docode();
                code[0] = '\0';
                printascii(32);
          //      Serial2.println();
            }
        }
    }



    //////////////////////////////
    // write if no more letters //
    //////////////////////////////

    if ((millis() - startttimelow) > (highduration * 6) && stop == LOW) {
        docode();
        code[0] = '\0';
        stop = HIGH;
    }

    /////////////////////////////////////
    // we will turn on and off the LED //
    // and the speaker                 //
    /////////////////////////////////////

    if (filteredstate == HIGH) {
        digitalWrite(cw_decoder_indicator, HIGH);
        rp2040.fifo.push_nb(0x101);
    }
    else {
        digitalWrite(cw_decoder_indicator, LOW);
        rp2040.fifo.push_nb(0x102);
    }

    realstatebefore = realstate;
    lasthighduration = highduration;
    filteredstatebefore = filteredstate;
}



#endif