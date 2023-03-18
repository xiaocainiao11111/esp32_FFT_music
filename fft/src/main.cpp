//已验证，esp32_FFT，快速傅里叶变换实现音乐频谱

#include <Arduino.h>
#include <arduinoFFT.h>
#include <Adafruit_SSD1306.h>


#define samples 64 //采样点数，2的N次幂
#define halfsamples samples/2 //32
#define NumofCopy halfsamples*sizeof(double) //位宽
#define Interval 128/(halfsamples) //4
Adafruit_SSD1306 display(100);
arduinoFFT FFT = arduinoFFT();

double vReal[samples];
double vImag[samples];
double vTemp[halfsamples];

void setup() {
  // setP32();  //设置32分频,加快AD采样
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {
//  Serial.println("start:"+String(micros()));
  for(int i=0;i<samples;i++)
  {
    vReal[i]=analogRead(0)-512;  //采样
    vImag[i] = 0.0;
  }
//  Serial.println("end:"+String(micros()));
  display.clearDisplay();
  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);//加窗，汉宁窗
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD);  //计算fft
  FFT.ComplexToMagnitude(vReal, vImag, samples);  //计算幅度
  for(int i=0;i<halfsamples-2;i++)
  {
    display.drawPixel(i*Interval,vTemp[halfsamples-i-1]*0.007+1, WHITE); //点
    display.drawLine(i*Interval, 0, i*Interval,vReal[halfsamples-i-1]*0.007, WHITE); //线
  }
  display.display(); //推送
  memcpy(vTemp, vReal, NumofCopy);
  delay(5);  //改为128点时可以注释掉，防止刷新太慢
}
 