#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
char buf[256];

static const char *WEEK_DAYS[] = {"日", "一", "二", "三", "四", "五", "六"};
String ip;

void inline startWifiConfig() {
  delay(500);
  WiFi.mode(WIFI_STA);
  WiFi.begin();
  for (int i = 0; i < 10; i++) {
    if (WiFi.status() == WL_CONNECTED) {
      break;
    }
    delay(500);
  }
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.beginSmartConfig();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_unifont_t_chinese3);
    u8g2.setCursor(0, 14);
    u8g2.print("用ESPTouch配网");
    u8g2.sendBuffer();
    while (!WiFi.smartConfigDone()) {
      delay(500);
    }
  }
  while (!WiFi.localIP().isSet()) {
    delay(200);
  }
  u8g2.setFont(u8g2_font_unifont_t_chinese3);
  u8g2.setCursor(0, 30);
  u8g2.print("WiFi连接成功");
  u8g2.sendBuffer();
}

void inline startConfigTime() {
  const int timeZone = 8 * 3600;
  configTime(timeZone, 0, "ntp6.aliyun.com", "cn.ntp.org.cn", "ntp.ntsc.ac.cn");
  while (time(nullptr) < 8 * 3600 * 2) {
    delay(500);
  }
  u8g2.setFont(u8g2_font_unifont_t_chinese3);
  u8g2.setCursor(0, 47);
  u8g2.print("成功获取时间");
  u8g2.sendBuffer();
  delay(500);
}

void setup() {
  Serial.begin(115200);
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_unifont_t_chinese3);
  u8g2.setCursor(0, 14);
  u8g2.print("开始连接WiFi");
  u8g2.sendBuffer();
  startWifiConfig();
  startConfigTime();
  ip = WiFi.localIP().toString();
}

void inline showCurrentTime() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_unifont_t_chinese1);
  u8g2.setCursor(0, 14);
  u8g2.print(ip.c_str());
  struct tm info;
  getLocalTime(&info);
  strftime(buf, 32, "%T", &info);
  u8g2.setFont(u8g2_font_logisoso24_tr);
  u8g2.setCursor(0, 44);
  u8g2.print(buf);
  sprintf(buf, "%04d-%02d-%02d 周%s", 1900 + info.tm_year, info.tm_mon + 1,
          info.tm_mday, WEEK_DAYS[info.tm_wday]);
  u8g2.setCursor(0, 61);
  u8g2.setFont(u8g2_font_unifont_t_chinese3);
  u8g2.print(buf);
  u8g2.sendBuffer();
}

void loop() {
  delay(300);
  showCurrentTime();
}