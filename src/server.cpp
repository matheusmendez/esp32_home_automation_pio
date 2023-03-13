#include "server.h"

const int PINS[] = {2, 32, 33, 25, 26};
const String PINS_LABELS[] = {"Luz Sala","Luz Quarto","Luz Cozinha","Motor","Tomada"};
int PINS_STATUS[] = {LOW,LOW,LOW,LOW,LOW};
float temperature = 0;
float humidity = 0;

WebServer server(SERVER_PORT);

void setupWiFi(void)
{
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to " + String(WIFI_SSID));
    IPAddress ipAddress;
    ipAddress.fromString(IP_ADDRESS);
    WiFi.config(ipAddress, WiFi.gatewayIP(), WiFi.subnetMask());
    Serial.println("IP address: " + String(WiFi.localIP()));
}

void setupServer(void)
{
    server.on("/", handleRoot);
    server.onNotFound(handleNotFound);
    server.begin();
}

void setupPins(void)
{
    for (int i = 0; i < sizeof(PINS) / sizeof(PINS[0]); i++)
    {
        pinMode(PINS[i], OUTPUT);
    }
}

void handleRoot(void)
{
    if (server.args() > 0)
    {
        execute(server.argName(0), server.arg(0));
    }
    readDHT(&temperature, &humidity);
    String html = "<html>";
    html.concat(head());
    html.concat(body());
    html.concat("</html>");
    server.send(200, "text/html; charset=UTF-8", html);
}

void handleNotFound(void)
{
    server.send(404, "text/plain", "Not Found");
}

void handleClient(void)
{
    server.handleClient();
}

void execute(String action, String value)
{
    if (action == "on" || action == "off")
    {
        int index = value.toInt();
        int status = action == "on" ? HIGH : LOW;
        digitalWrite(index, status);
        for (int i = 0; i < sizeof(PINS) / sizeof(PINS[0]); i++)
        {
            if (PINS[i] == index)
            {
                PINS_STATUS[i] = status;
                break;
            }
        }
    }
}

String head(void)
{
    return (F("<head>"
                "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                "<meta http-equiv='refresh' content='10;URL=/'>"
                "<style>"
                "body{"
                "text-align: center;"
                "font-family: Helvetica;"
                "font-size: 14px;"
                "color: #FFF;"
                "background-image: linear-gradient(to left, #4b00bb,#350069);"
                "}"
                "p{"
                "color:#FFF;"
                "font-size: 20px;"
                "}"
                "h1{"
                "color:#FFF;"
                "font-size: 40px;"
                "}"
                ".button{"
                "outline: none;"
                "display: block;"
                "border: 1px solid #555;"
                "border-radius:36px;"
                "width: 300px;"
                "height: 60px;"
                "margin: 20px;"
                "margin-left: auto;"
                "margin-right: auto;"
                "cursor: pointer;"
                "font-size: 20px;"
                "}"
                ".button_off{"
                "background-color:#555555;;"
                "color: #FFF;"
                "}"
                ".button_on{"
                "background-color:#2C5;"
                "color: #FFF;"
                "}"
                "</style>"
                "</head>"));
}

String body(void)
{
    String b = "<body>"
                "<h1>ESP32 Home Automation</h1>"
                "<p>Temperature: " + String(temperature) + " °C</p>"
                "<p>Humidity: " + String(humidity) + "%</p>";
    for (int i = 0; i < sizeof(PINS) / sizeof(PINS[0]); i++)
    {
        b.concat(button(PINS[i], i));
    }
    b.concat("</body>");
    return b;
}

String button(int number, int index)
{
    String label = String(number);
    String className = "button ";
    className += PINS_STATUS[index] == HIGH ? "button_on" : "button_off";
    String action = PINS_STATUS[index] == HIGH ? "off" : "on";
    return "<button class=\"" + className + "\"onclick=\"location.href='?" + action + "=" + label + "'\">" + PINS_LABELS[index] + "</button>";
}