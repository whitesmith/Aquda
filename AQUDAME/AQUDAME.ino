/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

/* Set these to your desired credentials. */
const char *ssid = "AQUDAME";
const char *password = "thereisnospoon";

String head = "<html>\r\n<head>\r\n<meta charset=\"UTF-8\">\r\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, maximum-scale=1\">\r\n<title>AQUDA<\/title>\r\n<style>\r\n*{\r\nfont-family: \"Lucida Console\", Monaco, monospace;\r\nbox-sizing:border-box;\r\n}\r\nbody{ \r\nbackground-color: Gainsboro; \r\nmargin:0;\r\nbox-shadow: inset 0px -200px 200px -200px rgba(255,255,255,0.5);\r\nmin-height: 100vh;\r\ncolor: AliceBlue; \r\n}\r\nmain{\r\ntext-align: center;\r\nmargin: 2em auto;\r\n}\r\n.logo{\r\nheight: 50px;\r\nwidth: auto;\r\n}\r\n.naming{\r\nheight: 45px;\r\nfill: SlateGray;\r\n}\r\n.top_bar{\r\nbackground: WhiteSmoke;\r\npadding: 1em;\r\nbox-shadow: 0px 5px 10px 0px rgba(0,0,0,0.05);\r\nz-index: 10;\r\n}\r\n.top_bar span{\r\nfont-size: 55px;\r\n}\r\n.user:before{\r\ncontent: \"Request assistance from\";\r\nopacity: .5;\r\n}\r\n.user{\r\nflex: 0 0 200px;\r\ncursor: pointer;\r\nposition: relative;\r\nwidth: 200px;\r\nheight: 200px;\r\nborder-radius: 50%;\r\nbackground: SlateGray;\r\nanimation: float 1s infinite ease-in-out alternate;\r\npadding: 3.5em 3em;\r\nfont-size: 0.8em;\r\nmargin: 3em;\r\nbox-shadow: 0px 0px 20px 0px rgba(0,0,0,0.4);\r\ntransition: box-shadow .2s, color .2s, background .2s;\r\n}\r\n.user:hover{\r\nbox-shadow: 0px 0px 20px 0px rgba(41,57,70,0.5);\r\nbackground: rgba(0,189,236,0.5);\r\ncolor: black;\r\n}\r\n.user:after{\r\ncontent:\"\";\r\nbackground: white;\r\nopacity: .1;\r\nposition: absolute;\r\nwidth: 20%;\r\nheight:10%;\r\nz-index: 1;\r\nleft: 15%;\r\ntop: 24%;\r\nborder-radius: 50%;\r\ntransform: rotate(-60deg);\r\nbox-shadow: 0px 0px 10px 10px white;\r\n}\r\n.user:nth-of-type(2) {animation-delay: .4s;}\r\n.user:nth-of-type(3) {animation-delay: .8s;}\r\n.user:nth-of-type(4) {animation-delay: .2s;}\r\n.user:nth-of-type(5) {animation-delay: .6s;}\r\n.user:nth-of-type(6) {animation-delay: .12s;}\r\n.user:nth-of-type(7) {animation-delay: .10s;}\r\n@keyframes float { 100% {transform: translateY(20px);box-shadow: 0px 0px 20px 0px rgba(0,0,0,0.1);} }\r\n.users_container{\r\nposition: absolute;\r\ntop: 50%;\r\nleft: 50%;\r\ntransform: translate(-50%, -50%);\r\nwidth: 100%;\r\ndisplay: flex;\r\nflex-direction: row;\r\njustify-content:space-around;\r\nflex-wrap: wrap;\r\n}\r\n.popup{\r\nposition: absolute;\r\ntop: 106px;\r\nleft:0;\r\nwidth: 100%;\r\nopacity: .99;\r\nbackground: SlateGray;\r\nheight: calc(100vh - 106px);\r\ndisplay: flex;\r\nflex-direction: column;\r\njustify-content: center;\r\ndisplay: none;\r\n}\r\n.login{\r\ndisplay: flex;\r\n}\r\n.close{\r\nbox-sizing: border-box;\r\nposition: absolute;\r\nright: 10px;\r\ntop: 10px;\r\nwidth: 30px;\r\nheight: 30px;\r\nbackground: whitesmoke;\r\nborder-radius: 50%;\r\ncolor: black;\r\npadding: 5px;\r\ncursor: pointer;\r\n}\r\n.close:after{\r\ncontent: \"\\2716\";\r\n}\r\n.close:hover:after{\r\ncontent: \"Cancel\";\r\n}\r\n.close:hover{\r\nwidth: 80px;\r\nborder-radius: 15px;\r\n}\r\n.login input,\r\nbutton{\r\nwidth: 80%;\r\nheight: 50px;\r\npadding: 1em;\r\nfont-size: 1em;\r\nfont-weight: bold;\r\ncolor: SlateGray;\r\nborder: none;\r\nborder-radius: 4px;\r\nmargin-bottom: 2em;\r\n}\r\nbutton{\r\nmargin: 30px auto 0 auto;\r\ncolor: whitesmoke;\r\nbackground: rgb(0,189,236);\r\nopacity: .1;\r\ncursor: pointer;\r\n}\r\nbutton:hover{\r\nopacity: 1;\r\n}\r\n.loading {\r\nposition: relative;\r\nleft: 50%;\r\ntransform: translate(-50%, 0%);\r\nwidth: 50px;\r\nheight: 50px;\r\nbackground-color: rgb(0,189,236);\r\nborder-radius: 50%;\r\n}  \r\n.loading:after {\r\ncontent: \'\';\r\nposition: absolute;\r\nborder-radius: 50%;\r\ntop: 50%;\r\nleft: 50%;\r\nborder: 0px solid SlateGray;\r\ntransform: translate(-50%, -50%);\r\nanimation: loading_animation 1000ms ease-out forwards infinite;\r\n}\r\n@keyframes loading_animation {0% {border: 0px solid SlateGray;} 20% {border: 8px solid SlateGray;width: 0%;height: 0%;} 100% {border: 8px solid SlateGray;width: 100%;height: 100%;}}\r\n.right{\r\nfloat: right;\r\ntext-align: right;\r\n}\r\n.right *{\r\ncolor: SlateGray;\r\n}\r\n.right h4{\r\nmargin: 6px 0 6px 0;\r\n}\r\n.right a{\r\nfont-size: 10px;\r\n}\r\n<\/style>\r\n<\/head>";
String nav = "<body>\r\n<nav class=\"top_bar\">\r\n<span>&#128561;<\/span>\r\n<svg class=\"naming\" xmlns=\"http:\/\/www.w3.org\/2000\/svg\" viewBox=\"0 0 289 80\"><path d=\"M50.545 69.226q-1 .8-2.3.7-1.2-.1-2.3-.7l-29.1-17q-.7-.4-1.3 0-.5.4-.7 1l-4.1 13.6q-.7 2.4-3.4 2.4h-4.9q-2.9 0-2.1-2.8l20-62.5q.5-1.6 1.3-2.1.9-.6 2.3-.6h7q1.4 0 2.2.6.9.5 1.4 2.1l19.1 59.8q.7 2.3-.8 3.6l-2.3 1.9zm-22.1-55.3q-.3-1.1-1-1.1t-1 1.1l-7.9 26.4q-.4 1.3-.1 2 .3.6 1 1l18.8 11q1.1.7 1.6.2.5-.6.3-1.3l-11.7-39.3zm75.807 65.3q-2.6 1.5-4.1-1.1l-5.4-9.3q-4.2 1.4-8.8 1.4-5.1 0-9.4-1.7-4.3-1.7-7.5-4.7-3.1-3-4.9-7.2-1.7-4.3-1.7-9.4v-24q0-5.1 1.7-9.3 1.8-4.3 4.9-7.3 3.2-3 7.5-4.7 4.3-1.7 9.4-1.7 5.1 0 9.4 1.7 4.3 1.7 7.4 4.7 3.2 3 4.9 7.3 1.8 4.2 1.8 9.3v24q0 5.3-1.9 9.6t-5.1 7.3l5.5 9.5q.7 1.2.3 2.3-.3 1.1-1.4 1.8l-2.6 1.5zm-26.8-40.7q-.7-1.2-.4-2.3.4-1.1 1.5-1.8l2.6-1.5q2.6-1.5 4.1 1.1l12.3 21.7q1.1-1.7 1.7-3.8.7-2.2.7-4.7v-24q0-6-3.6-10t-10.4-4q-6.8 0-10.4 4-3.6 4-3.6 10v24q0 6 3.6 10t10.4 4q2.1 0 4.1-.5l-12.6-22.2zm81.985-34.3q0-1.4.8-2.2.8-.8 2.2-.8h3.5q1.4 0 2.2.8.8.8.8 2.2v43q0 5.1-1.8 9.4-1.7 4.2-4.9 7.2-3.1 3-7.4 4.7-4.3 1.7-9.4 1.7-5.1 0-9.4-1.7-4.3-1.7-7.5-4.7-3.1-3-4.9-7.2-1.7-4.3-1.7-9.4v-43q0-1.4.8-2.2.8-.8 2.2-.8h3.5q1.4 0 2.2.8.8.8.8 2.2v43q0 6 3.6 10t10.4 4q6.8 0 10.4-4 3.6-4 3.6-10v-43zm25.96 65q-1.4 0-2.2-.8-.8-.8-.8-2.2v-62q0-1.4.8-2.2.8-.8 2.2-.8h17.5q5.5 0 10 1.9 4.6 1.8 7.8 5t4.9 7.6q1.8 4.4 1.8 9.5v20q0 5.1-1.8 9.5-1.7 4.4-4.9 7.6-3.2 3.2-7.8 5.1-4.5 1.8-10 1.8h-17.5zm32.5-44q0-3.5-1.1-6.3t-3.1-4.7q-2-2-4.8-3-2.7-1-6-1h-10q-1 0-1 1v48q0 1 1 1h10q3.3 0 6-1 2.8-1 4.8-2.9t3.1-4.7q1.1-2.8 1.1-6.4v-20zm67.61 44q-1 .8-2.3.7-1.2-.1-2.3-.7l-29.1-17q-.7-.4-1.3 0-.5.4-.7 1l-4.1 13.6q-.7 2.4-3.4 2.4h-4.9q-2.9 0-2.1-2.8l20-62.5q.5-1.6 1.3-2.1.9-.6 2.3-.6h7q1.4 0 2.2.6.9.5 1.4 2.1l19.1 59.8q.7 2.3-.8 3.6l-2.3 1.9zm-22.1-55.3q-.3-1.1-1-1.1t-1 1.1l-7.9 26.4q-.4 1.3-.1 2 .3.6 1 1l18.8 11q1.1.7 1.6.2.5-.6.3-1.3l-11.7-39.3z\"\/><\/svg>\r\n<div class=\"right\">\r\n<!-- <div class=\"username\"><h4>Renato<\/h4><\/div> -->\r\n<div class=\"logout\"><a href=\"http:\/\/192.168.4.1\/login?DISCONNECT=YES\">Logout<\/a><\/div>\r\n<\/div>\r\n<\/nav>\r\n<main>";
String users = "<section class=\"users_container\">\r\n<div class=\"user\" onclick=\"requestHelp(this);\">\r\n<h2>Renato de Le\u00E3o<\/h2>\r\n<\/div>\r\n<div class=\"user\" onclick=\"requestHelp(this);\">\r\n<h2>Z\u00E9 Ribeiro<\/h2>\r\n<\/div>\r\n<div class=\"user\" onclick=\"requestHelp(this);\">\r\n<h2>Rafael Jegundo<\/h2>\r\n<\/section>\r\n<section class=\"popup\" id=\"js-requestHelp\">\r\n<div class=\"close\" onclick=\"closePopup();\"><\/div>\r\n<h4>Requesting HELP from<\/h4>\r\n<h6 class=\"loading\"><\/h6>\r\n<h2 id=\"js-userHelper\"><\/h2>\r\n<button>I really really need help!<\/button>\r\n<\/section>";
String usersP1 = "<section class=\"users_container\">\r\n";
String usersP2 = "</section><section class=\"popup\" id=\"js-requestHelp\">\r\n<div class=\"close\" onclick=\"closePopup();\"><\/div>\r\n<h4>Requesting HELP from<\/h4>\r\n<h6 class=\"loading\"><\/h6>\r\n<h2 id=\"js-userHelper\"><\/h2>\r\n<button>I really really need help!<\/button>\r\n<\/section>";
String login = "<section class=\"login popup\">\r\n<form action=\'http:\/\/192.168.4.1\/login\' method=\'POST\'>\r\n<input type=\'text\' name=\'USERNAME\' placeholder=\'Insert Full Name\' required=\"\"><br>\r\n<input type=\'submit\' name=\'SUBMIT\' value=\'ENTER\'>\r\n<\/form>\r\n<\/section>";
String footer = "<\/main>\r\n<\/body>\r\n<script type=\"text\/javascript\">\r\nvar currentHelper;\r\nfunction closePopup(){\r\n\tvar requestHelpPopup = document.getElementById(\"js-requestHelp\");\r\n\trequestHelpPopup.style.display = \"none\";\r\n\tvar IP = currentHelper.dataset.user;\r\n\tvar xhttp = new XMLHttpRequest();\r\n\txhttp.open(\"GET\", IP + \"\/cancel?user=\" + currentHelper.querySelector(\"h2\").innerHTML, true);\r\n\txhttp.send();\r\n\tcurrentHelper = null;\r\n}\r\nfunction requestHelp(object){\r\n\tcurrentHelper = object\r\n\tdocument.getElementById(\"js-userHelper\").innerText = currentHelper.querySelector(\"h2\").innerHTML;\r\n\tvar IP = currentHelper.dataset.user;\r\n\tdocument.getElementById(\"js-requestHelp\").style.display = \"flex\";\r\n\tvar xhttp = new XMLHttpRequest();\r\n\txhttp.open(\"GET\", IP + \"\/help\", true);\r\n\txhttp.send();\r\n}\r\n<\/script>\r\n<\/html>";

String username = "";

String registeredUsers[5];
String registeredUsersIP[5];
int arrayPointer = 0;

Servo flag;

ESP8266WebServer server(80);

//Check if header is present and correct
bool is_authentified() {
  Serial.println("Enter is_authentified");
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
    if (cookie.indexOf("USER=1") != -1) {
      Serial.println("Authentification Successful");
      username = cookie.substring(6);
      return true;
    }
  }
  Serial.println("Authentification Failed");
  return false;
}

//login page, also called for disconnect
void handleLogin() {
  String msg;
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
  }
  if (server.hasArg("DISCONNECT")) {
    Serial.println("Disconnection");
    String header = "HTTP/1.1 301 OK\r\nSet-Cookie: USER=0\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  if (server.hasArg("USERNAME")) {
    String header = "HTTP/1.1 301 OK\r\nSet-Cookie: USER=1" + server.arg("USERNAME") + "\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    registeredUsers[arrayPointer] = String(server.arg("USERNAME"));
    IPAddress clientIP = server.client().remoteIP();
    String IP = "http://" + String(clientIP[0]) + "." + String(clientIP[1]) + "." + String(clientIP[2]) + "." + String(clientIP[3]);
    registeredUsersIP[arrayPointer] = IP;
    arrayPointer++;
    Serial.println("Log in Successful");
    return;
  }
  server.send(200, "text/html", head + nav + login + footer);
}

void handleRoot() {
  Serial.println("Enter handleRoot");
  if (!is_authentified()) {
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  server.send(200, "text/html", head + nav + getUsers() + footer);
}
void handleNeedHelp() {
  flag.write(1);
  server.send(200, "text/html", "Need Help Sent");
}

void handleUrgentHelp() {
  flag.write(180);
  server.send(200, "text/html", "Urgent Help Request Sent");
}

void handleCancel() {
  flag.write(85);
  server.send(200, "text/html", "Urgent Help Request Sent");
}

String constructUser(String username, String IP) {
  return "<div data-user=\"" + IP + "\" class=\"user\" onclick=\"requestHelp(this);\">\r\n<h2>" + username + "<\/h2>\r\n<\/div>";
}
String getUsers() {
  String content = usersP1;
  for(int i = 0; i<arrayPointer; i++){
    content += constructUser(registeredUsers[i], registeredUsersIP[i]);
  }
  content += usersP2;
  return content;
}


void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  if (!MDNS.begin("AQUDA")) {
    Serial.println("Error setting up MDNS responder!");
  } else {
    Serial.println("mDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  server.on("/help", handleNeedHelp);
  server.on("/cancel", handleCancel);
  server.on("/urgent", handleUrgentHelp);

  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent", "Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize );
  server.begin();
  Serial.println("HTTP server started");

  flag.attach(4);
  flag.write(85);
}

void loop() {
  server.handleClient();
  yield();
}

