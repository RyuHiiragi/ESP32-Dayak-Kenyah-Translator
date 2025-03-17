#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "dictionary.h"

const char* ssid = "YOUR_WIFI_SSID";      // Replace with your WiFi SSID
const char* password = "YOUR_WIFI_PASS";  // Replace with your WiFi password

WebServer server(80);
StaticJsonDocument<8192> dict;

// HTML/CSS/JS Interface
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Dayak Kenyah Translator</title>
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css">
    <style>
        @import url('https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600&display=swap');
        :root {
            --primary: #6ba3be;
            --secondary: #475069;
            --background: #2d3342;
            --text-main: #ffffff;
            --text-secondary: #b0b6c3;
            --hover: #8ab4c7;
        }
        * { margin:0; padding:0; box-sizing:border-box; }
        body {
            font-family: 'Inter', sans-serif;
            background: var(--background);
            color: var(--text-main);
            line-height:1.6;
            min-height:100vh;
            display:flex;
            flex-direction:column;
        }
        .container {
            flex:1;
            padding:2rem;
            max-width:1200px;
            margin:0 auto;
        }
        .header {
            text-align:center;
            margin-bottom:2rem;
        }
        .logo {
            width:150px;
            border-radius:8px;
            margin-bottom:1rem;
        }
        .title {
            font-size:2.5rem;
            color:var(--primary);
        }
        .description {
            color:var(--text-secondary);
            font-size:0.9rem;
            margin-top:1rem;
        }
        .translator-box {
            background:var(--secondary);
            padding:2rem;
            border-radius:12px;
            box-shadow:0 4px 6px rgba(0,0,0,0.1);
        }
        .lang-indicator {
            font-weight:500;
            color:var(--text-secondary);
            margin:1rem 0;
        }
        .input-group {
            display:flex;
            gap:1rem;
            margin:1rem 0;
        }
        .text-input {
            flex:1;
            padding:1rem;
            background:var(--background);
            color:var(--text-main);
            border-radius:8px;
            min-height:100px;
            resize:vertical;
        }
        .swap-btn {
            background:var(--primary);
            color:white;
            border:none;
            padding:0.8rem 1.2rem;
            border-radius:8px;
            cursor:pointer;
            font-size:1.2rem;
        }
        .swap-btn:hover { background:var(--hover); }
        .output-box {
            background:var(--background);
            padding:1rem;
            border-radius:8px;
            min-height:100px;
            margin-top:1rem;
        }
        .social-section {
            text-align:center;
            padding:2rem;
            background:var(--secondary);
            margin-top:auto;
        }
        .social-link {
            color:var(--text-main);
            margin:0 0.5rem;
            font-size:1.2rem;
        }
        .social-link:hover { color:var(--primary); }
        .footer {
            color:var(--text-secondary);
            font-size:0.8rem;
        }
        @media (max-width:768px) {
            .input-group { flex-direction:column; }
            .swap-btn { width:100%; margin-top:0.5rem; }
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <img src="https://i.pinimg.com/736x/cc/82/6d/cc826d6dccc9dcf66760320f295a688f.jpg" class="logo">
            <h1 class="title">Dayak Kenyah Translator</h1>
            <p class="description">
                Kenyah language translator for Indonesian ↔ Dayak Kenyah with 200+ vocabulary entries
            </p>
        </div>

        <div class="translator-box">
            <div class="lang-indicator">
                Translate from: <span id="currentLang">Indonesian</span>
            </div>
            
            <div class="input-group">
                <textarea class="text-input" 
                          id="inputText"
                          placeholder="Enter Indonesian text..."></textarea>
                <button class="swap-btn" onclick="swapLanguage()">↔</button>
            </div>

            <div class="lang-indicator">
                Translation Result (<span id="targetLang">Dayak Kenyah</span>):
            </div>
            <div class="output-box">
                <p id="outputText"></p>
            </div>
        </div>
    </div>

    <div class="social-section">
        <div class="social-links">
            <a href="https://github.com/RyuHiiragi" class="social-link" target="_blank"><i class="fab fa-github"></i></a>
            <a href="https://www.instagram.com/kysukamieayam" class="social-link" target="_blank"><i class="fab fa-instagram"></i></a>
        </div>
        <div class="footer">
            <p>Muhammad Rizky Saputra</p>
            <p>XI TJKT 2</p>
        </div>
    </div>

    <script>
        let currentLang = 'id';

        function swapLanguage() {
            currentLang = currentLang === 'id' ? 'dyk' : 'id';
            document.getElementById('currentLang').textContent = 
                currentLang === 'id' ? 'Indonesian' : 'Dayak Kenyah';
            document.getElementById('targetLang').textContent = 
                currentLang === 'id' ? 'Dayak Kenyah' : 'Indonesian';
            document.getElementById('inputText').placeholder = 
                `Enter ${currentLang === 'id' ? 'Indonesian' : 'Dayak'} text...`;
            document.getElementById('inputText').value = '';
            document.getElementById('outputText').textContent = '';
        }

        function translateText() {
            const input = document.getElementById('inputText').value.trim().toLowerCase();
            if (!input) return;

            fetch(`/translate?text=${encodeURIComponent(input)}&lang=${currentLang}`)
                .then(response => response.text())
                .then(result => {
                    document.getElementById('outputText').textContent = result;
                });
        }

        document.getElementById('inputText').addEventListener('input', translateText);
    </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", index_html);
}

void handleTranslate() {
  if (!server.hasArg("text") || !server.hasArg("lang")) {
    server.send(400, "text/plain", "Missing parameters");
    return;
  }

  String text = server.arg("text");
  String lang = server.arg("lang");
  text.toLowerCase();

  String result = "";

  if (lang == "id") {
    // Check for full phrase translation
    if (dict.containsKey(text)) {
      result = dict[text].as<const char*>();
    } else {
      // Split into words and translate individually
      int start = 0;
      int end = text.indexOf(' ');
      while (end != -1) {
        String word = text.substring(start, end);
        if (dict.containsKey(word)) {
          result += String(dict[word].as<const char*>()) + " ";
        } else {
          result += word + " ";
        }
        start = end + 1;
        end = text.indexOf(' ', start);
      }
      String lastWord = text.substring(start);
      result += dict.containsKey(lastWord) ? String(dict[lastWord].as<const char*>()) : lastWord;
    }
  } else {
    // Translate Dayak to Indonesian
    bool foundFull = false;
    for (JsonPair pair : dict.as<JsonObject>()) {
      if (pair.value().as<String>().equalsIgnoreCase(text)) {
        result = pair.key().c_str();
        foundFull = true;
        break;
      }
    }
    if (!foundFull) {
      int start = 0;
      int end = text.indexOf(' ');
      while (end != -1) {
        String word = text.substring(start, end);
        bool found = false;
        for (JsonPair pair : dict.as<JsonObject>()) {
          if (pair.value().as<String>().equalsIgnoreCase(word)) {
            result += String(pair.key().c_str()) + " ";
            found = true;
            break;
          }
        }
        if (!found) result += word + " ";
        start = end + 1;
        end = text.indexOf(' ', start);
      }
      String lastWord = text.substring(start);
      bool foundLast = false;
      for (JsonPair pair : dict.as<JsonObject>()) {
        if (pair.value().as<String>().equalsIgnoreCase(lastWord)) {
          result += pair.key().c_str();
          foundLast = true;
          break;
        }
      }
      if (!foundLast) result += lastWord;
    }
  }

  server.send(200, "text/plain", result);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());
  
  if (deserializeJson(dict, dictionary_json)) {
    Serial.println("Failed to load dictionary!");
    return;
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/translate", HTTP_GET, handleTranslate);
  server.begin();
}

void loop() {
  server.handleClient();
}
