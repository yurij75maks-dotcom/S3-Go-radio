// ============================================
// ESP32 Radio Web Interface
// web_interface.h
// ============================================

#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

#include <WebServer.h>
#include <LittleFS.h>
#include <WiFi.h>

extern WebServer server;
extern String currentStreamName;
extern String currentPlaylistFile;
extern String currentBackground;
extern int currentVolume;
extern int streamCount;
extern int currentStreamIndex;
extern bool radioStarted;
extern bool isPaused;
extern int needlePosLX, needlePosLY, needlePosRX, needlePosRY;
extern int needleThickness, needleLenMain, needleLenRed, needleCY;
extern int stationNameX, stationNameY;
extern int streamTitle1X, streamTitle1Y;
extern int streamTitle2X, streamTitle2Y;
extern int bitrateX, bitrateY;
extern int clockX, clockY;
extern int volumeX, volumeY;
extern int needleMinAngle;
extern int needleMaxAngle;
extern int needleUpSpeed;
extern int needleDownSpeed;

extern String getStreamName(int idx);
extern void selectStream(int idx);
extern void nextStream();
extern void prevStream();
extern void loadPlaylistMetadata(String filename);
extern void loadBackground();
extern void redrawScreen();
extern void drawVolumeDisplay();
extern void saveSettings();

String getBackgroundsList() {
  String json = "[";
  File root = LittleFS.open("/");
  File file = root.openNextFile();
  bool first = true;
  
  while(file){
    String name = String(file.name());
    if(name.endsWith(".jpg") || name.endsWith(".jpeg")){
      if(!first) json += ",";
      json += "{\"name\":\"" + name + "\",\"selected\":" + String(name == currentBackground ? "true" : "false") + "}";
      first = false;
    }
    file = root.openNextFile();
  }
  json += "]";
  return json;
}

String getPlaylistsList() {
  String json = "[";
  File root = LittleFS.open("/");
  File file = root.openNextFile();
  bool first = true;
  
  while(file){
    String name = String(file.name());
    if(name.endsWith(".csv")){
      if(!first) json += ",";
      json += "{\"name\":\"" + name + "\",\"selected\":" + String(name == currentPlaylistFile ? "true" : "false") + "}";
      first = false;
    }
    file = root.openNextFile();
  }
  json += "]";
  return json;
}

String getStreamsList() {
  String json = "[";
  for(int i = 0; i < streamCount; i++){
    if(i > 0) json += ",";
    json += "{\"idx\":" + String(i) + ",\"name\":\"" + getStreamName(i) + "\",\"playing\":" + String(i == currentStreamIndex ? "true" : "false") + "}";
  }
  json += "]";
  return json;
}

String getPlayerStatus() {
  String json = "{";
  json += "\"station\":\"" + currentStreamName + "\",";
  json += "\"playing\":" + String(radioStarted && !isPaused ? "true" : "false") + ",";
  json += "\"paused\":" + String(isPaused ? "true" : "false") + ",";
  json += "\"volume\":" + String(currentVolume);
  json += "}";
  return json;
}

String getNetworkInfo() {
  String json = "{";
  json += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
  json += "\"rssi\":" + String(WiFi.RSSI()) + ",";
  json += "\"bg\":\"" + currentBackground + "\",";
  json += "\"playlist\":\"" + currentPlaylistFile + "\",";
  json += "\"streams\":" + String(streamCount) + ",";
  json += "\"status\":\"" + String(radioStarted ? (isPaused ? "PAUSED" : "PLAYING") : "STOPPED") + "\",";
  json += "\"heap\":" + String(ESP.getFreeHeap() / 1024) + ",";
  json += "\"psram\":" + String(ESP.getFreePsram() / 1024) + ",";
  json += "\"fsTotal\":" + String(LittleFS.totalBytes() / 1024) + ",";
  json += "\"fsUsed\":" + String(LittleFS.usedBytes() / 1024) + ",";
  json += "\"fsFree\":" + String((LittleFS.totalBytes() - LittleFS.usedBytes()) / 1024);
  json += "}";
  return json;
}

String getWebInterfaceHTML() {

  String html = "<!DOCTYPE html><html lang='ru'><head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>ESP32 Radio Control</title>";
  html += "<style>";
  html += "* { margin: 0; padding: 0; box-sizing: border-box; }";
  html += "html,body { height: 100%; }";
  html += "body { font-family: 'Segoe UI', Arial, sans-serif; background: linear-gradient(135deg, #1a1a1a 0%, #2d2d2d 100%); color: #fff; min-height: 100vh; overflow-x: hidden; }";
  html += ".topbar { position: fixed; top: 0; left: 0; width: 100%; height: 60px; background: rgba(20,20,20,0.95); border-bottom: 2px solid #00ffff; display: flex; align-items: center; gap: 8px; padding: 6px 10px; overflow-x: auto; z-index: 1000; }";
  html += ".topbar::-webkit-scrollbar { height: 6px; }";
  html += ".topbar::-webkit-scrollbar-thumb { background: #00ffff; border-radius: 4px; }";
  html += ".menu-item { flex: 0 0 auto; padding: 8px 14px; cursor: pointer; transition: all 0.25s ease; border-bottom: 3px solid transparent; display: flex; align-items: center; gap: 8px; border-radius: 8px; white-space: nowrap; }";
  html += ".menu-item:hover { background: rgba(0,255,255,0.06); border-bottom-color: #00ffff; }";
  html += ".menu-item.active { background: rgba(0,255,255,0.14); border-bottom-color: #00ffff; color: #00ffff; }";
  html += ".menu-icon { font-size: 18px; width: 22px; text-align: center; }";
  html += ".main-content { margin-top: 74px; padding: 20px; min-height: calc(100vh - 74px); }";
  html += ".content-section { display: none; }";
  html += ".content-section.active { display: block; }";
  html += ".player-row { display: flex; gap: 16px; align-items: flex-start; justify-content: flex-start; flex-wrap: wrap; }";
  html += ".now-playing { background: rgba(0, 255, 255, 0.06); padding: 18px; border-radius: 10px; border: 2px solid #00ffff; min-width: 280px; flex: 1 1 320px; }";
  html += ".now-playing h2 { color: #00ffff; font-size: 14px; margin-bottom: 8px; text-transform: uppercase; letter-spacing: 2px; }";
  html += ".station-name { color: #00ff00; font-size: 20px; font-weight: 700; margin-bottom: 8px; }";
  html += ".status-text { color: #ff9900; font-size: 15px; margin-bottom: 10px; }";
  html += ".player-controls { display: flex; gap: 10px; align-items: center; justify-content: flex-start; margin-bottom: 8px; }";
  html += ".player-btn { background: linear-gradient(135deg, #00ffff, #00cccc); color: #000; border: none; padding: 10px 16px; border-radius: 8px; font-size: 18px; cursor: pointer; transition: transform 0.18s ease, box-shadow 0.18s ease; }";
  html += ".player-btn:hover { transform: translateY(-3px); box-shadow: 0 6px 20px rgba(0,255,255,0.2); }";
  html += ".volume-control { margin-top: 6px; }";
  html += ".volume-label { color: #00ffff; font-weight: bold; font-size: 14px; }";
  html += ".volume-value { color: #00ff00; font-size: 16px; font-weight: bold; }";
  html += ".playlist-container { background: rgba(42,42,42,0.85); padding: 18px; border-radius: 10px; max-height: 880px; border: 2px solid #00ffff; overflow-y: auto; flex: 1 1 400px; }";
  html += ".playlist-title { color: #00ffff; font-size: 16px; margin-bottom: 12px; }";
  html += ".station-item { padding: 12px 14px; margin: 8px 0; border-radius: 8px; background: rgba(50,50,50,0.45); cursor: pointer; transition: transform 0.18s ease, background 0.18s ease; display: flex; align-items: center; gap: 12px; }";
  html += ".station-item:hover { transform: translateX(6px); background: rgba(0,255,255,0.06); }";
  html += ".station-item.playing { background: rgba(0,255,0,0.12); border-left: 4px solid #00ff00; }";
  html += ".station-bullet { width: 12px; height: 12px; border-radius: 50%; border: 2px solid #00ffff; }";
  html += ".station-item.playing .station-bullet { background: #00ff00; border-color: #00ff00; }";
  html += ".section { background: rgba(42,42,42,0.85); padding: 18px; border-radius: 10px; margin-bottom: 16px; border: 2px solid #00ffff; }";
  html += ".section h2 { color: #00ffff; font-size: 18px; margin-bottom: 12px; }";
  html += ".file-row { display: flex; gap: 10px; margin-bottom: 10px; align-items: center; }";
  html += ".file-btn { flex: 1; background: #222; color: #fff; padding: 10px 12px; border-radius: 6px; border: 2px solid #444; cursor: pointer; font-weight: 700; }";
  html += ".file-btn.selected { background: #00ff00; color: #000; }";
  html += ".delete-btn { background: #ff4444; color: #fff; padding: 8px 12px; border-radius: 6px; border: none; cursor: pointer; font-size: 13px; }";
  html += ".delete-btn:hover { background: #ff2222; }";
  html += ".info-line { display: flex; justify-content: space-between; padding: 10px; background: rgba(0,0,0,0.28); border-radius: 6px; margin: 8px 0; }";
  html += ".info-label { color: #00ffff; }";
  html += ".info-value { color: #00ff00; }";
  html += "input[type='range'] { width: 100%; height: 8px; background: #555; border-radius: 6px; outline: none; -webkit-appearance: none; }";
  html += "input[type='range']::-webkit-slider-thumb { -webkit-appearance: none; width: 22px; height: 22px; border-radius: 50%; background: linear-gradient(135deg, #00ffff, #0099ff); cursor: pointer; box-shadow: 0 0 10px rgba(0,255,255,0.35); }";
  html += "input[type='file'], input[type='submit'], button { border-radius: 6px; border: none; padding: 10px; font-size: 14px; }";
  html += "input[type='file'] { width: 100%; background: #333; color: #fff; margin-bottom: 10px; border: 2px solid #555; }";
  html += "input[type='submit'], button { background: linear-gradient(135deg, #00ffff, #00cccc); color: #000; font-weight: 700; cursor: pointer; }";
  html += "input[type='submit']:hover, button:hover { transform: translateY(-2px); box-shadow: 0 6px 18px rgba(0,255,255,0.18); }";
  html += ".settings-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(250px, 1fr)); gap: 16px; }";
  html += ".setting-item { background: rgba(0,0,0,0.28); padding: 12px; border-radius: 6px; }";
  html += ".setting-label { color: #00ffff; font-size: 13px; margin-bottom: 6px; display: block; }";
  html += ".setting-value { color: #00ff00; font-weight: 700; font-size: 14px; }";
  html += "@media (max-width: 800px) { .now-playing { flex-basis: 100%; } }";
  html += "@media (max-width: 520px) { .topbar { height: 56px; } .menu-item { padding: 8px 10px; font-size: 14px; } }";
  html += "</style>";

  // JavaScript
  html += "<script>";
  html += "function setActiveMenu(name){ document.querySelectorAll('.menu-item').forEach(e=>e.classList.remove('active'));";
  html += " var el = document.querySelector('.menu-item[data-section=\"'+name+'\"]'); if(el) el.classList.add('active'); }";
  html += "function showSection(section){ document.querySelectorAll('.content-section').forEach(s=>s.classList.remove('active'));";
  html += " var target = document.getElementById(section); if(target) target.classList.add('active');";
  html += " setActiveMenu(section); localStorage.setItem('lastSection', section); }";
  html += "window.addEventListener('load', function(){ var last = localStorage.getItem('lastSection'); if(last && document.getElementById(last)) showSection(last); else showSection('player'); });";
  
  // Функция безопасного fetch
  html += "function safeFetch(path, callback){ fetch(path).then(r=>r.text()).then(data=>{ if(callback) callback(data); }).catch(err=>console.log('fetch err',err)); }";
  
  // Обновление плеера
  html += "function updatePlayer(){ safeFetch('/api/player', function(data){ var p = JSON.parse(data); ";
  html += "document.getElementById('stationName').innerText = p.station; ";
  html += "document.getElementById('statusText').innerText = p.playing ? '▶️ PLAYING' : (p.paused ? '⏸️ PAUSED' : 'ℹ️ STOPPED'); ";
  html += "document.getElementById('playBtn').innerText = (p.playing && !p.paused) ? '⏸️' : '▶️'; ";
  html += "document.getElementById('volDisplay').innerText = p.volume; ";
  html += "document.querySelector('.volume-control input').value = p.volume; }); }";
  
  // Обновление списка станций
  html += "function updateStreams(){ safeFetch('/api/streams', function(data){ var streams = JSON.parse(data); ";
  html += "var html = ''; streams.forEach(function(s){ ";
  html += "var activeClass = s.playing ? ' playing' : ''; ";
  html += "html += '<div class=\"station-item'+activeClass+'\" onclick=\"selectStream('+s.idx+')\">'; ";
  html += "html += '<span class=\"station-bullet\"></span>' + s.name + '</div>'; }); ";
  html += "document.getElementById('streamsList').innerHTML = html; }); }";
  
  // Обновление фонов
  html += "function updateBackgrounds(){ safeFetch('/api/backgrounds', function(data){ var bgs = JSON.parse(data); ";
  html += "var html = ''; bgs.forEach(function(bg){ ";
  html += "var selClass = bg.selected ? ' selected' : ''; ";
  html += "html += '<div class=\"file-row\">'; ";
  html += "html += '<button class=\"file-btn'+selClass+'\" onclick=\"selectBg(\\''+bg.name+'\\')\">🖼️ '+bg.name+'</button>'; ";
  html += "html += '<button class=\"delete-btn\" onclick=\"deleteBg(\\''+bg.name+'\\')\">🗑️</button></div>'; ";
  html += "}); document.getElementById('bgList').innerHTML = html; }); }";

  // Обновление плейлистов
  html += "function updatePlaylists(){ safeFetch('/api/playlists', function(data){ var pls = JSON.parse(data); ";
  html += "var html = ''; pls.forEach(function(pl){ ";
  html += "var selClass = pl.selected ? ' selected' : ''; ";
  html += "html += '<div class=\"file-row\">'; ";
  html += "html += '<button class=\"file-btn'+selClass+'\" onclick=\"selectPlaylist(\\''+pl.name+'\\')\">📋 '+pl.name+'</button>'; ";
  html += "html += '<button class=\"delete-btn\" onclick=\"deletePlaylist(\\''+pl.name+'\\')\">🗑️</button></div>'; ";
  html += "}); document.getElementById('plList').innerHTML = html; }); }";

  // Обновление сетевой информации
  html += "function updateNetworkInfo(){ safeFetch('/api/network', function(data){ var n = JSON.parse(data); ";
  html += "document.getElementById('netIP').innerText = n.ip; ";
  html += "document.getElementById('netRSSI').innerText = n.rssi + ' dBm'; ";
  html += "document.getElementById('netBg').innerText = n.bg; ";
  html += "document.getElementById('netPlaylist').innerText = n.playlist; ";
  html += "document.getElementById('netStreams').innerText = n.streams; ";
  html += "document.getElementById('netStatus').innerText = n.status; ";
  html += "document.getElementById('netHeap').innerText = n.heap + ' KB'; ";
  html += "document.getElementById('netPSRAM').innerText = n.psram + ' KB'; ";
  html += "document.getElementById('netFSTotal').innerText = n.fsTotal + ' KB'; ";
  html += "document.getElementById('netFSUsed').innerText = n.fsUsed + ' KB'; ";
  html += "document.getElementById('netFSFree').innerText = n.fsFree + ' KB'; }); }";
  
  // Действия
  html += "function selectStream(idx){ safeFetch('/selectstream?idx='+idx, function(){ updatePlayer(); updateStreams(); }); }";
  html += "function selectBg(name){ safeFetch('/selectbg?bg='+encodeURIComponent(name), function(){ updateBackgrounds(); }); }";
  html += "function deleteBg(name){ if(confirm('Delete '+name+'?')) safeFetch('/deletebg?bg='+encodeURIComponent(name), function(){ updateBackgrounds(); }); }";
  html += "function selectPlaylist(name){ safeFetch('/selectplaylist?pl='+encodeURIComponent(name), function(){ updatePlaylists(); updateStreams(); updatePlayer(); }); }";
  html += "function deletePlaylist(name){ if(confirm('Delete '+name+'?')) safeFetch('/deleteplaylist?pl='+encodeURIComponent(name), function(){ updatePlaylists(); updateStreams(); }); }";
  html += "function playPause(){ safeFetch('/play', function(){ setTimeout(updatePlayer, 300); }); }";
  html += "function nextStation(){ safeFetch('/next', function(){ setTimeout(updatePlayer, 300); setTimeout(updateStreams, 300); }); }";
  html += "function prevStation(){ safeFetch('/prev', function(){ setTimeout(updatePlayer, 300); setTimeout(updateStreams, 300); }); }";
  html += "function setVolume(v){ safeFetch('/setvolume?v='+v, function(){ document.getElementById('volDisplay').innerText = v; }); }";
  
  // Авто-обновление Network Info каждые 5 секунд
  html += "setInterval(function(){ if(document.getElementById('network').classList.contains('active')) updateNetworkInfo(); }, 5000);";
  
  html += "</script>";
  html += "</head><body>";

  // Topbar
  html += "<div class='topbar'>";
  html += "  <div class='menu-item active' data-section='player' onclick=\"showSection('player')\"><span class='menu-icon'>🎵</span> Player</div>";
  html += "  <div class='menu-item' data-section='upload' onclick=\"showSection('upload'); updatePlaylists();\"><span class='menu-icon'>📤</span> Upload</div>";
  html += "  <div class='menu-item' data-section='backgrounds' onclick=\"showSection('backgrounds'); updateBackgrounds();\"><span class='menu-icon'>🖼️</span> Backgrounds</div>";
  // html += "  <div class='menu-item' data-section='playlists' onclick=\"showSection('playlists'); updatePlaylists();\"><span class='menu-icon'>📋</span> Playlists</div>";
  html += "  <div class='menu-item' data-section='needles' onclick=\"showSection('needles')\"><span class='menu-icon'>🎯</span> VU Needles</div>";
  html += "  <div class='menu-item' data-section='textpos' onclick=\"showSection('textpos')\"><span class='menu-icon'>📝</span> Text Positions</div>";
  html += "  <div class='menu-item' data-section='network' onclick=\"showSection('network'); updateNetworkInfo();\"><span class='menu-icon'>🌐</span> Network</div>";
  html += "  <div class='menu-item' data-section='colors' onclick=\"showSection('colors')\"><span class='menu-icon'>🎨</span> Colors</div>";
  html += "  <div class='menu-item' onclick=\"safeFetch('/updatetft'); alert('TFT Updated!')\"><span class='menu-icon'>🔄</span> Update TFT</div>";
  html += "</div>";

  html += "<div class='main-content'>";

  // === PLAYER SECTION ===
  html += "<div id='player' class='content-section'>";
  html += "  <div class='player-row'>";
  
  // Левая колонка: плеер + менеджер плейлистов
  html += "    <div style='display:flex; flex-direction:column; gap:16px; flex:1 1 400px;'>";
  html += "      <div class='now-playing'>";
  html += "        <h2>S3-Go! light</h2>";
  html += "        <div class='station-name' id='stationName'>" + currentStreamName + "</div>";
  String statusText = (!radioStarted) ? "ℹ️ STOPPED" : (isPaused ? "⏸️ PAUSED" : "▶️ PLAYING");
  html += "        <div class='status-text' id='statusText'>" + statusText + "</div>";
  html += "        <div class='player-controls'>";
  html += "          <button class='player-btn' onclick='prevStation()'>⮜</button>";
  String playIcon = (radioStarted && !isPaused) ? "⏸️" : "▶️";
  html += "          <button class='player-btn' id='playBtn' onclick='playPause()'>" + playIcon + "</button>";
  html += "          <button class='player-btn' onclick='nextStation()'>⮞</button>";
  html += "        </div>";
  html += "        <div class='volume-control'>";
  html += "          <label class='volume-label'>🔊 Volume: <span class='volume-value' id='volDisplay'>" + String(currentVolume) + "</span></label>";
  html += "          <input type='range' min='0' max='21' value='" + String(currentVolume) + "' oninput='setVolume(this.value)'>";
  html += "        </div>";
  html += "      </div>";
  
  // Менеджер плейлистов под плеером
  html += "      <div class='section' style='margin:0;'>";
  html += "        <h2>📋 Manage Playlists</h2>";
  html += "        <div id='plList2'>";
  {
    File root3 = LittleFS.open("/");
    File file3 = root3.openNextFile();
    bool playlistFound2 = false;
    while (file3) {
      String name = String(file3.name());
      if (name.endsWith(".csv")) {
        playlistFound2 = true;
        String selectedClass = (name == currentPlaylistFile) ? " selected" : "";
        html += "<div class='file-row'>";
        html += "<button class='file-btn" + selectedClass + "' onclick='selectPlaylist(\"" + name + "\")'>📋 " + name + "</button>";
        html += "<button class='delete-btn' onclick='deletePlaylist(\"" + name + "\")'>🗑️</button>";
        html += "</div>";
      }
      file3 = root3.openNextFile();
    }
    if (!playlistFound2) {
      html += "<p style='color:#ff9900'>⚠️ No playlists found</p>";
    }
  }
  html += "        </div>";
  html += "      </div>";
  html += "    </div>";
  
  // Правая колонка: список станций
  html += "    <div class='playlist-container'>";
  html += "      <div class='playlist-title'>📻 PLAYLIST</div>";
  html += "      <div id='streamsList'>";
  if (streamCount > 0) {
    for (int i = 0; i < streamCount; i++) {
      String activeClass = (i == currentStreamIndex) ? " playing" : "";
      html += "<div class='station-item" + activeClass + "' onclick='selectStream(" + String(i) + ")'>";
      html += "<span class='station-bullet'></span>" + getStreamName(i) + "</div>";
    }
  } else {
    html += "<p style='color:#ff9900'>⚠️ No streams</p>";
  }
  html += "      </div>";
  html += "    </div>";
  
  html += "  </div>";
  html += "</div>";

  // === UPLOAD SECTION ===
  html += "<div id='upload' class='content-section'>";
  html += "  <div class='section'>";
  html += "    <h2>📤 Upload Files</h2>";
  html += "    <form method='POST' action='/upload' enctype='multipart/form-data' onsubmit='setTimeout(function(){ updatePlaylists(); updateBackgrounds(); }, 1000);'>";
  html += "      <input type='file' name='file' accept='.jpg,.jpeg,.csv' required>";
  html += "      <input type='submit' value='📤 Upload File'>";
  html += "    </form>";
  html += "    <p style='color:#888;margin-top:10px;font-size:13px'>Supported: JPG/JPEG (480x320) or CSV playlist (Name[TAB]URL[TAB]Genre)</p>";
  html += "  </div>";
  html += "  <div class='section'>";
  html += "    <h2>📋 Manage Playlists</h2>";
  html += "    <div id='plList'>";
  {
    File root2 = LittleFS.open("/");
    File file2 = root2.openNextFile();
    bool playlistFound = false;
    while (file2) {
      String name = String(file2.name());
      if (name.endsWith(".csv")) {
        playlistFound = true;
        String selectedClass = (name == currentPlaylistFile) ? " selected" : "";
        html += "<div class='file-row'>";
        html += "<button class='file-btn" + selectedClass + "' onclick='selectPlaylist(\"" + name + "\")'>📋 " + name + "</button>";
        html += "<button class='delete-btn' onclick='deletePlaylist(\"" + name + "\")'>🗑️</button>";
        html += "</div>";
      }
      file2 = root2.openNextFile();
    }
    if (!playlistFound) {
      html += "<p style='color:#ff9900'>⚠️ No playlists found</p>";
    }
  }
  html += "    </div>";
  html += "  </div>";
  html += "</div>";

  // === BACKGROUNDS SECTION ===
  html += "<div id='backgrounds' class='content-section'>";
  html += "  <div class='section'>";
  html += "    <h2>🖼️ Select Background</h2>";
  html += "    <div id='bgList'>";
  {
    File rootBg = LittleFS.open("/");
    File fileBg = rootBg.openNextFile();
    bool hasBackgrounds = false;
    while (fileBg) {
      String name = String(fileBg.name());
      if (name.endsWith(".jpg") || name.endsWith(".jpeg")) {
        hasBackgrounds = true;
        String selectedClass = (name == currentBackground) ? " selected" : "";
        html += "<div class='file-row'>";
        html += "<button class='file-btn" + selectedClass + "' onclick='selectBg(\"" + name + "\")'>🖼️ " + name + "</button>";
        html += "<button class='delete-btn' onclick='deleteBg(\"" + name + "\")'>🗑️</button>";
        html += "</div>";
      }
      fileBg = rootBg.openNextFile();
    }
    if (!hasBackgrounds) {
      html += "<p style='color:#ff9900'>⚠️ No backgrounds found</p>";
    }
  }
  html += "    </div>";
  html += "  </div>";
  html += "</div>";

  // === PLAYLISTS SECTION ===
  // html += "<div id='playlists' class='content-section'>";
  // html += "  <div class='section'>";
  // html += "    <h2>📋 Manage Playlists</h2>";
  // html += "    <div id='plList2'>";
  // {
  //   File root3 = LittleFS.open("/");
  //   File file3 = root3.openNextFile();
  //   bool playlistFound2 = false;
  //   while (file3) {
  //     String name = String(file3.name());
  //     if (name.endsWith(".csv")) {
  //       playlistFound2 = true;
  //       String selectedClass = (name == currentPlaylistFile) ? " selected" : "";
  //       html += "<div class='file-row'>";
  //       html += "<button class='file-btn" + selectedClass + "' onclick='selectPlaylist(\"" + name + "\")'>📋 " + name + "</button>";
  //       html += "<button class='delete-btn' onclick='deletePlaylist(\"" + name + "\")'>🗑️</button>";
  //       html += "</div>";
  //     }
  //     file3 = root3.openNextFile();
  //   }
  //   if (!playlistFound2) {
  //     html += "<p style='color:#ff9900'>⚠️ No playlists found</p>";
  //   }
  // }
  // html += "    </div>";
  // html += "  </div>";
  // html += "</div>";

  // === NEEDLES SECTION ===
  html += "<div id='needles' class='content-section'>";
  html += "  <div class='section'>";
  html += "    <h2>🎯 VU Needle Settings</h2>";
  html += "    <div class='settings-grid'>";
  // Left position
  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Left X: <span class='setting-value' id='lxVal'>" + String(needlePosLX) + "</span></label>";
  html += "        <input type='range' min='0' max='480' id='lx' value='" + String(needlePosLX) + "' oninput=\"document.getElementById('lxVal').innerText=this.value; safeFetch('/setneedle?lx='+this.value+'&ly='+document.getElementById('ly').value+'&rx='+document.getElementById('rx').value+'&ry='+document.getElementById('ry').value)\">";
  html += "      </div>";
  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Left Y: <span class='setting-value' id='lyVal'>" + String(needlePosLY) + "</span></label>";
  html += "        <input type='range' min='0' max='480' id='ly' value='" + String(needlePosLY) + "' oninput=\"document.getElementById('lyVal').innerText=this.value; safeFetch('/setneedle?lx='+document.getElementById('lx').value+'&ly='+this.value+'&rx='+document.getElementById('rx').value+'&ry='+document.getElementById('ry').value)\">";
  html += "      </div>";
  // Right position
  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Right X: <span class='setting-value' id='rxVal'>" + String(needlePosRX) + "</span></label>";
  html += "        <input type='range' min='0' max='480' id='rx' value='" + String(needlePosRX) + "' oninput=\"document.getElementById('rxVal').innerText=this.value; safeFetch('/setneedle?lx='+document.getElementById('lx').value+'&ly='+document.getElementById('ly').value+'&rx='+this.value+'&ry='+document.getElementById('ry').value)\">";
  html += "      </div>";
  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Right Y: <span class='setting-value' id='ryVal'>" + String(needlePosRY) + "</span></label>";
  html += "        <input type='range' min='0' max='480' id='ry' value='" + String(needlePosRY) + "' oninput=\"document.getElementById('ryVal').innerText=this.value; safeFetch('/setneedle?lx='+document.getElementById('lx').value+'&ly='+document.getElementById('ly').value+'&rx='+document.getElementById('rx').value+'&ry='+this.value)\">";
  html += "      </div>";
  // === NEEDLE ANGLES ===
  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Min Angle: <span class='setting-value' id='minAngleVal'>" + String(needleMinAngle) + "°</span></label>";
  html += "        <input type='range' min='-360' max='360' id='minAngle' value='" + String(needleMinAngle) + "' oninput=\"document.getElementById('minAngleVal').innerText=this.value+'°'; safeFetch('/setneedleangles?minAngle='+this.value+'&maxAngle='+document.getElementById('maxAngle').value)\">";
  html += "      </div>";

  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Max Angle: <span class='setting-value' id='maxAngleVal'>" + String(needleMaxAngle) + "°</span></label>";
  html += "        <input type='range' min='-360' max='360' id='maxAngle' value='" + String(needleMaxAngle) + "' oninput=\"document.getElementById('maxAngleVal').innerText=this.value+'°'; safeFetch('/setneedleangles?minAngle='+document.getElementById('minAngle').value+'&maxAngle='+this.value)\">";
  html += "      </div>";

  // Добавь подсказку
  html += "      <div style='grid-column:1/-1; background:rgba(0,255,255,0.1); padding:10px; border-radius:6px; margin-top:10px;'>";
  html += "        <p style='color:#00ffff; font-size:12px; margin:0;'>💡 <strong>Min Angle</strong>: положение при 0% VU<br><strong>Max Angle</strong>: положение при 100% VU<br>Рекомендуется: Min=-200° to -240°, Max=-40° to -80°</p>";
  html += "      </div>";

  // === NEEDLE SPEEDS ===
  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Up Speed: <span class='setting-value' id='upSpeedVal'>" + String(needleUpSpeed) + "</span></label>";
  html += "        <input type='range' min='1' max='20' id='upSpeed' value='" + String(needleUpSpeed) + "' oninput=\"document.getElementById('upSpeedVal').innerText=this.value; safeFetch('/setneedlespeeds?upSpeed='+this.value+'&downSpeed='+document.getElementById('downSpeed').value)\">";
  html += "      </div>";

  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Down Speed: <span class='setting-value' id='downSpeedVal'>" + String(needleDownSpeed) + "</span></label>";
  html += "        <input type='range' min='1' max='10' id='downSpeed' value='" + String(needleDownSpeed) + "' oninput=\"document.getElementById('downSpeedVal').innerText=this.value; safeFetch('/setneedlespeeds?upSpeed='+document.getElementById('upSpeed').value+'&downSpeed='+this.value)\">";
  html += "      </div>";

  // Обнови подсказку
  html += "      <div style='grid-column:1/-1; background:rgba(0,255,255,0.1); padding:10px; border-radius:6px; margin-top:10px;'>";
  html += "        <p style='color:#00ffff; font-size:12px; margin:0;'>💡 <strong>Up Speed</strong>: скорость подъема стрелки (1-20)<br><strong>Down Speed</strong>: скорость опускания стрелки (1-10)<br><strong>Min Angle</strong>: положение при 0% VU<br><strong>Max Angle</strong>: положение при 100% VU<br>Рекомендуется: Min=-200° to -240°, Max=-40° to -80°</p>";
  html += "      </div>";

  // Needle appearance
  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Thickness: <span class='setting-value' id='thkVal'>" + String(needleThickness) + "</span></label>";
  html += "        <input type='range' min='1' max='9' id='thk' value='" + String(needleThickness) + "' oninput=\"document.getElementById('thkVal').innerText=this.value; safeFetch('/setneedleapp?thk='+this.value+'&main='+document.getElementById('main').value+'&red='+document.getElementById('red').value+'&cy='+document.getElementById('cy').value)\">";
  html += "      </div>";
  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Main Length: <span class='setting-value' id='mainVal'>" + String(needleLenMain) + "</span></label>";
  html += "        <input type='range' min='20' max='180' id='main' value='" + String(needleLenMain) + "' oninput=\"document.getElementById('mainVal').innerText=this.value; safeFetch('/setneedleapp?thk='+document.getElementById('thk').value+'&main='+this.value+'&red='+document.getElementById('red').value+'&cy='+document.getElementById('cy').value)\">";
  html += "      </div>";
  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Red Length: <span class='setting-value' id='redVal'>" + String(needleLenRed) + "</span></label>";
  html += "        <input type='range' min='5' max='40' id='red' value='" + String(needleLenRed) + "' oninput=\"document.getElementById('redVal').innerText=this.value; safeFetch('/setneedleapp?thk='+document.getElementById('thk').value+'&main='+document.getElementById('main').value+'&red='+this.value+'&cy='+document.getElementById('cy').value)\">";
  html += "      </div>";
  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Center Y: <span class='setting-value' id='cyVal'>" + String(needleCY) + "</span></label>";
  html += "        <input type='range' min='0' max='260' id='cy' value='" + String(needleCY) + "' oninput=\"document.getElementById('cyVal').innerText=this.value; safeFetch('/setneedleapp?thk='+document.getElementById('thk').value+'&main='+document.getElementById('main').value+'&red='+document.getElementById('red').value+'&cy='+this.value)\">";
  html += "      </div>";
  html += "    </div>";
  html += "  </div>";
  html += "</div>";

  // === TEXT POSITIONS SECTION ===
  html += "<div id='textpos' class='content-section'>";
  html += "  <div class='section'>";
  html += "    <h2>📝 Text Positions</h2>";
  html += "    <div class='settings-grid'>";
  // Station Name
  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Station Name X: <span class='setting-value' id='stationXVal'>" + String(stationNameX) + "</span></label>";
  html += "        <input type='range' min='0' max='480' value='" + String(stationNameX) + "' oninput=\"document.getElementById('stationXVal').innerText=this.value; safeFetch('/settextpos?stationX='+this.value)\">";
  html += "      </div>";
  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Station Name Y: <span class='setting-value' id='stationYVal'>" + String(stationNameY) + "</span></label>";
  html += "        <input type='range' min='0' max='480' value='" + String(stationNameY) + "' oninput=\"document.getElementById('stationYVal').innerText=this.value; safeFetch('/settextpos?stationY='+this.value)\">";
  html += "      </div>";
  // Title 1
  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Title Line 1 X: <span class='setting-value' id='title1XVal'>" + String(streamTitle1X) + "</span></label>";
  html += "        <input type='range' min='0' max='480' value='" + String(streamTitle1X) + "' oninput=\"document.getElementById('title1XVal').innerText=this.value; safeFetch('/settextpos?title1X='+this.value)\">";
  html += "      </div>";
  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Title Line 1 Y: <span class='setting-value' id='title1YVal'>" + String(streamTitle1Y) + "</span></label>";
  html += "        <input type='range' min='0' max='480' value='" + String(streamTitle1Y) + "' oninput=\"document.getElementById('title1YVal').innerText=this.value; safeFetch('/settextpos?title1Y='+this.value)\">";
  html += "      </div>";
  // Title 2
  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Title Line 2 X: <span class='setting-value' id='title2XVal'>" + String(streamTitle2X) + "</span></label>";
  html += "        <input type='range' min='0' max='480' value='" + String(streamTitle2X) + "' oninput=\"document.getElementById('title2XVal').innerText=this.value; safeFetch('/settextpos?title2X='+this.value)\">";
  html += "      </div>";
  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Title Line 2 Y: <span class='setting-value' id='title2YVal'>" + String(streamTitle2Y) + "</span></label>";
  html += "        <input type='range' min='0' max='480' value='" + String(streamTitle2Y) + "' oninput=\"document.getElementById('title2YVal').innerText=this.value; safeFetch('/settextpos?title2Y='+this.value)\">";
  html += "      </div>";
  // Bitrate
  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Bitrate X: <span class='setting-value' id='bitrateXVal'>" + String(bitrateX) + "</span></label>";
  html += "        <input type='range' min='0' max='480' value='" + String(bitrateX) + "' oninput=\"document.getElementById('bitrateXVal').innerText=this.value; safeFetch('/settextpos?bitrateX='+this.value)\">";
  html += "      </div>";
  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Bitrate Y: <span class='setting-value' id='bitrateYVal'>" + String(bitrateY) + "</span></label>";
  html += "        <input type='range' min='0' max='480' value='" + String(bitrateY) + "' oninput=\"document.getElementById('bitrateYVal').innerText=this.value; safeFetch('/settextpos?bitrateY='+this.value)\">";
  html += "      </div>";
  // Clock
  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Clock Y: <span class='setting-value' id='clockYVal'>" + String(clockY) + "</span></label>";
  html += "        <input type='range' min='0' max='480' value='" + String(clockY) + "' oninput=\"document.getElementById('clockYVal').innerText=this.value; safeFetch('/settextpos?clockY='+this.value)\">";
  html += "        <p style='color:#888;font-size:11px;margin-top:4px'>Clock X is auto-centered</p>";
  html += "      </div>";
  // Volume
  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Volume X: <span class='setting-value' id='volumeXVal'>" + String(volumeX) + "</span></label>";
  html += "        <input type='range' min='0' max='480' value='" + String(volumeX) + "' oninput=\"document.getElementById('volumeXVal').innerText=this.value; safeFetch('/settextpos?volumeX='+this.value)\">";
  html += "      </div>";
  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Volume Y: <span class='setting-value' id='volumeYVal'>" + String(volumeY) + "</span></label>";
  html += "        <input type='range' min='0' max='480' value='" + String(volumeY) + "' oninput=\"document.getElementById('volumeYVal').innerText=this.value; safeFetch('/settextpos?volumeY='+this.value)\">";
  html += "      </div>";
  html += "    </div>";
  html += "  </div>";
  html += "</div>";

  // === COLORS SECTION ===
  html += "<div id='colors' class='content-section'>";
  html += "  <div class='section'>";
  html += "    <h2>🎨 Text & Needle Colors</h2>";
  html += "    <div style='display:grid; grid-template-columns:1fr 1fr 1fr; gap:12px; align-items:end;'>";
    
  // Функция для конвертации RGB565 в HEX
  html += "<script>";
  html += "function rgb565ToHex(rgb565) {";
  html += "  var r = ((rgb565 >> 11) & 0x1F) << 3;";
  html += "  var g = ((rgb565 >> 5) & 0x3F) << 2;";
  html += "  var b = (rgb565 & 0x1F) << 3;";
  html += "  return '#' + ((1 << 24) + (r << 16) + (g << 8) + b).toString(16).slice(1).toUpperCase();";
  html += "}";
  html += "function hexToRgb565(hex) {";
  html += "  hex = hex.replace('#', '');";
  html += "  var r = parseInt(hex.substr(0,2), 16) >> 3;";
  html += "  var g = parseInt(hex.substr(2,2), 16) >> 2;";
  html += "  var b = parseInt(hex.substr(4,2), 16) >> 3;";
  html += "  return (r << 11) | (g << 5) | b;";
  html += "}";
  html += "</script>";

  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Station Name</label>";
  html += "        <input type='color' id='colorStation' value='#F7E540' onchange=\"safeFetch('/setcolors?station='+hexToRgb565(this.value).toString(16))\" style='width:100%; height:40px;'>";
  html += "      </div>";

  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Stream Title</label>";
  html += "        <input type='color' id='colorTitle' value='#F7E540' onchange=\"safeFetch('/setcolors?title='+hexToRgb565(this.value).toString(16))\" style='width:100%; height:40px;'>";
  html += "      </div>";

  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Bitrate</label>";
  html += "        <input type='color' id='colorBitrate' value='#F7E540' onchange=\"safeFetch('/setcolors?bitrate='+hexToRgb565(this.value).toString(16))\" style='width:100%; height:40px;'>";
  html += "      </div>";

  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Volume</label>";
  html += "        <input type='color' id='colorVolume' value='#F7E540' onchange=\"safeFetch('/setcolors?volume='+hexToRgb565(this.value).toString(16))\" style='width:100%; height:40px;'>";
  html += "      </div>";

  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Clock</label>";
  html += "        <input type='color' id='colorClock' value='#00FFFF' onchange=\"safeFetch('/setcolors?clock='+hexToRgb565(this.value).toString(16))\" style='width:100%; height:40px;'>";
  html += "      </div>";

  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Needle Main</label>";
  html += "        <input type='color' id='colorNeedleMain' value='#000000' onchange=\"safeFetch('/setcolors?needleMain='+hexToRgb565(this.value).toString(16))\" style='width:100%; height:40px;'>";
  html += "      </div>";

  html += "      <div class='setting-item'>";
  html += "        <label class='setting-label'>Needle Red</label>";
  html += "        <input type='color' id='colorNeedleRed' value='#FF0000' onchange=\"safeFetch('/setcolors?needleRed='+hexToRgb565(this.value).toString(16))\" style='width:100%; height:40px;'>";
  html += "      </div>";

  html += "    </div>";
  html += "    <button onclick='updateColorInputs()' style='margin-top:12px; padding:10px; background:#444; color:white; border:none; border-radius:6px; cursor:pointer;'>🔄 Load Current Colors</button>";
  html += "    <p style='color:#888; margin-top:12px; font-size:13px;'>💡 Colors update immediately. Use 'Update TFT' button if needed.</p>";
  html += "  </div>";
  html += "</div>";

  // Функция для обновления цветов при загрузке страницы
  html += "<script>";
  html += "function updateColorInputs() {";
  html += "  fetch('/api/colors').then(r=>r.json()).then(colors => {";
  html += "    document.getElementById('colorStation').value = rgb565ToHex(colors.station);";
  html += "    document.getElementById('colorTitle').value = rgb565ToHex(colors.title);";
  html += "    document.getElementById('colorBitrate').value = rgb565ToHex(colors.bitrate);";
  html += "    document.getElementById('colorVolume').value = rgb565ToHex(colors.volume);";
  html += "    document.getElementById('colorClock').value = rgb565ToHex(colors.clock);";
  html += "    document.getElementById('colorNeedleMain').value = rgb565ToHex(colors.needleMain);";
  html += "    document.getElementById('colorNeedleRed').value = rgb565ToHex(colors.needleRed);";
  html += "  });";
  html += "}";
  html += "// Автоматически загружаем цвета при открытии раздела";
  html += "document.addEventListener('DOMContentLoaded', function() {";
  html += "  var observer = new MutationObserver(function(mutations) {";
  html += "    mutations.forEach(function(mutation) {";
  html += "      if (mutation.target.id === 'colors' && mutation.target.classList.contains('active')) {";
  html += "        updateColorInputs();";
  html += "      }";
  html += "    });";
  html += "  });";
  html += "  observer.observe(document.getElementById('colors'), { attributes: true, attributeFilter: ['class'] });";
  html += "});";
  html += "</script>";

  // === NETWORK SECTION ===
  html += "<div id='network' class='content-section'>";
  html += "  <div class='section'>";
  html += "    <h2>🌐 Network Information</h2>";
  html += "    <div class='info-line'><span class='info-label'>IP Address:</span><span class='info-value' id='netIP'>" + WiFi.localIP().toString() + "</span></div>";
  html += "    <div class='info-line'><span class='info-label'>WiFi Signal:</span><span class='info-value' id='netRSSI'>" + String(WiFi.RSSI()) + " dBm</span></div>";
  html += "    <div class='info-line'><span class='info-label'>Current Background:</span><span class='info-value' id='netBg'>" + currentBackground + "</span></div>";
  html += "    <div class='info-line'><span class='info-label'>Current Playlist:</span><span class='info-value' id='netPlaylist'>" + currentPlaylistFile + "</span></div>";
  html += "    <div class='info-line'><span class='info-label'>Streams Count:</span><span class='info-value' id='netStreams'>" + String(streamCount) + "</span></div>";
  html += "    <div class='info-line'><span class='info-label'>Radio Status:</span><span class='info-value' id='netStatus'>" + String(radioStarted ? (isPaused ? "PAUSED" : "PLAYING") : "STOPPED") + "</span></div>";
  html += "    <div class='info-line'><span class='info-label'>Free Heap:</span><span class='info-value' id='netHeap'>" + String(ESP.getFreeHeap() / 1024) + " KB</span></div>";
  html += "    <div class='info-line'><span class='info-label'>Free PSRAM:</span><span class='info-value' id='netPSRAM'>" + String(ESP.getFreePsram() / 1024) + " KB</span></div>";
  html += "    <div class='info-line'><span class='info-label'>LittleFS Total:</span><span class='info-value' id='netFSTotal'>" + String(LittleFS.totalBytes() / 1024) + " KB</span></div>";
  html += "    <div class='info-line'><span class='info-label'>LittleFS Used:</span><span class='info-value' id='netFSUsed'>" + String(LittleFS.usedBytes() / 1024) + " KB</span></div>";
  html += "    <div class='info-line'><span class='info-label'>LittleFS Free:</span><span class='info-value' id='netFSFree'>" + String((LittleFS.totalBytes() - LittleFS.usedBytes()) / 1024) + " KB</span></div>";
  html += "  </div>";
  html += "</div>";

  html += "</div>"; // end main-content
  html += "</body></html>";


  return html;
}

#endif // WEB_INTERFACE_H