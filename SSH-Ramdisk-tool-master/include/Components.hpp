#ifndef Components_hpp
#define Components_hpp

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <getopt.h>

struct loaders {
  std::string name;
  std::string path;
};

struct info {
  std::string link;
  std::string boardcfg;
  std::string temp;
  std::string ramdisk;
  std::string trustcache;
  std::string Kernel;
  std::string Devicetree;
  std::string rdpath;
  std::string usrin;
  std::string chipid;
};

struct keys {
  std::string Buildid;
  std::string page;
  std::string iv;
  std::string key;
};

namespace load {

info inpt;
info ipsw;
loaders ibss;
loaders ibec;

// parsers didn't work for me so i made this crappy parser

static std::string Time(){
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::string s(30, '\0');
    std::strftime(&s[0], s.size(), "%H:%M:%S", std::localtime(&now));
    return s;
}

/*
static struct option longopts[] = {
  {"help", no_argument, NULL, 'h'},
  { "identifier", required_argument, NULL, 'd' },
  { "version", required_argument, NULL, 'i' },
  { "board", required_argument, NULL, 'b' },
  { "shsh", required_argument, NULL, 's' },
  { "pwn", no_argument, NULL, 'p' },
  { "verbose",   no_argument, NULL, 'v' },
  {"custom", no_argument, NULL, 'c'},
  { NULL, 0, NULL, 0 }
};
*/

//std::ifstream dirchk((std::string("Patched_") + identifier + "_" + version + "/ramdisk").c_str());
//std::ifstream chk((std::string("Patched_") + identifier + "_" + version + "iBSS.img4").c_str());

void GetManifest(std::string identifier, std::string version){ // gets buildmanifest and ipswlink
mkdir((std::string("WD_") + identifier + "_" + version).c_str(), S_IRWXU);
chdir((std::string("WD_") + identifier + "_" + version).c_str());


 system((std::string("curl -s -LO \"https://api.ipsw.me/v2.1/") + identifier + "/" + version + "/info.json\"").c_str());
  std::ifstream infojson("info.json");
   while(getline(infojson, ipsw.temp)){
    if(ipsw.temp.find("\"url\": \"")!= std::string::npos){
     ipsw.temp.erase(0, 12);
      for(int i = 0; i < ipsw.temp.size(); i++){
       if(ipsw.temp[i] == '"'){
        ipsw.temp.erase(i);
         ipsw.link = ipsw.temp;
          infojson.close();
       }
      }
     }
    }
system((std::string("pzb -g ") + "BuildManifest.plist " + ipsw.link).c_str());
}

std::string iBSS(std::string board){

chdir((std::string("WD_") + identifier + "_" + version).c_str());

std::ifstream Manifest("BuildManifest.plist");
 while(getline(Manifest, ibss.path)) {
  if(ibss.path.find(board) != std::string::npos) {
   for(int i = 0; i < 4000 && getline(Manifest, ibss.path); i++) {
    if(ibss.path.find("iBSS.")!= std::string::npos) {
     Manifest.close();
      ibss.path.erase(0, 14);
       for(int y = 0; y < ibss.path.size(); y++) {
        if(ibss.path[y] == '<') {
         ibss.path.erase(y);
          system((std::string("pzb -g ") + ibss.path + " " + ipsw.link).c_str());
           ibss.path.erase(0, 13);
            ibss.name = ibss.path;
        }
       }
      }
     }
    }
   }
  return ibss.name;
  }

std::string iBEC(std::string board){

chdir((std::string("WD_") + identifier + "_" + version).c_str());

std::ifstream Manifest("BuildManifest.plist");
 while (getline(Manifest, ibec.path)) {
  if(ibec.path.find(board) != std::string::npos) {
   for(int t = 0; t < 4000 && getline(Manifest, ibec.path); t++) {
    if(ibec.path.find("iBEC.") != std::string::npos){
     Manifest.close();
      ibec.path.erase(0, 14);
       for(int e = 0; e < ibec.path.size(); e++) {
        if(ibec.path[e] == '<') {
         ibec.path.erase(e);
          system((std::string("pzb -g ") + ibec.path + " " + ipsw.link).c_str());
           ibec.path.erase(0, 13);
            ibec.name = ibec.path;
         }
        }
       }
      }
     }
    }
  return ibec.name;
   }

std::string buildid(){

keys keyspage;
std::string bldid;

chdir((std::string("WD_") + identifier + "_" + version).c_str());
 std::ifstream json("info.json");
  while(getline(json, bldid)){
   if(bldid.find("\"buildid\": \"") != std::string::npos){
    bldid.erase(0, 16);
     for(int b = 0; b < bldid.size(); b++){
      if(bldid[b] == '"'){
       bldid.erase(b);
       keyspage.Buildid = bldid;
     }
    }
   }
  }
  return keyspage.Buildid;
}

void Getkeyspage(std::string identifier, std::string version){

keys keyspage;

for(int r = 0; r < version.size(); r++){
 if(version[r] == '.'){
  version.erase(r);
   system((std::string("curl -s -LO \"https://www.theiphonewiki.com/wiki/Firmware_Keys/") + version + ".x" + "\"").c_str());
    std::ifstream x((version + ".x"));
     while(getline(x, keyspage.page)){
      if(keyspage.page.find(identifier) != std::string::npos){
       if(keyspage.page.find(buildid()) != std::string::npos){
        x.close();
         for(int w = 0; w < keyspage.page.size(); w++){
          if(keyspage.page[w] == '/'){
           keyspage.page.erase(0, w);
            for(int p = 0; p < keyspage.page.size(); p++){
             if(keyspage.page[p] == '"'){
              keyspage.page.erase(p);
               system((std::string("curl -s \"https://www.theiphonewiki.com") + keyspage.page + "\" -o keys.html").c_str());
                system((std::string("rm -r ") + version + ".x").c_str());
         }
        }
       }
      }
     }
    }
   }
  }
 }
}

std::string kernel(std::string board) {

std::string krn;

chdir((std::string("WD_") + identifier + "_" + version).c_str());
 std::ifstream kern("BuildManifest.plist");
  while(getline(kern, krn)) {
   if(krn.find(board) != std::string::npos) {
    for(int i = 0; i < 3268 && getline(kern, krn); i++) {
     if(krn.find("kernelcache.") != std::string::npos) {
      kern.close();
       krn.erase(0, 14);
        for(int t = 0; t < krn.size(); t++){
         if(krn[t] == '<'){
          krn.erase(t);
           system((std::string("pzb -g ") + krn + " " + ipsw.link).c_str());
            ipsw.Kernel = krn;
       }
      }
     }
    }
   }
  }
return ipsw.Kernel;
}

std::string DeviceTree(std::string board) {

std::string Devtre;

chdir((std::string("WD_") + identifier + "_" + version).c_str());
 std::ifstream manifest("BuildManifest.plist");
  while(getline(manifest, Devtre)) {
   if(Devtre.find(board) != std::string::npos) {
    for(int o = 0; o < 3268 && getline(manifest, Devtre); o++) {
     if(Devtre.find("DeviceTree.") != std::string::npos) {
      manifest.close();
       Devtre.erase(0, 14);
        for(int y = 0; y < Devtre.size(); y++) {
         if(Devtre[y] == '<') {
          Devtre.erase(y);
           system((std::string("pzb -g ") + Devtre + " " + ipsw.link).c_str());
            Devtre.erase(0, 19);
             ipsw.Devicetree = Devtre;
       }
      }
     }
    }
   }
  }
return ipsw.Devicetree;
}

std::string ramdisk(std::string identifier, std::string version) {

std::string rdisk;
chdir((std::string("WD_") + identifier + "_" + version).c_str());

std::ifstream keys("keys.html");
 if(!keys) {
   std::cout << "[e] It seems the script could not get the firmware keys. This is common on macs lower than catalina. Sorry but please update your mac to catalina or higher (Or you specified your device with lowercase p :) )" << '\n';
   exit(1);
 }
 while(getline(keys, rdisk)) {
  if(rdisk.find("keypage-restoreramdisk") != std::string::npos) {
   for(int i = 0; i < rdisk.size(); i++) {
    if(rdisk[i] == '0') {
     rdisk.erase(0, i);
      for(int o = 0; o < rdisk.size(); o++) {
       if(rdisk[o] == '<') {
        rdisk.erase(o);
         system((std::string("pzb -g ") + rdisk + " " + ipsw.link).c_str());
          ipsw.ramdisk = rdisk;
      }
     }
    }
   }
  }
 }
return ipsw.ramdisk;
}

std::string trsc(std::string identifier, std::string version) {

chdir((std::string("WD_") + identifier + "_" + version).c_str());
 system((std::string("pzb -g ") + "Firmware/" + ipsw.ramdisk + ".trustcache " + ipsw.link).c_str());
  ipsw.trustcache = (ipsw.ramdisk + ".trustcache");

return ipsw.trustcache;
}

std::string iBSSIV() { // unused functs

keys iBSSiv;

chdir((std::string("WD_") + identifier + "_" + version).c_str());
 std::ifstream keys("keys.html");
  while(getline(keys, iBSSiv.page)) {
   if(iBSSiv.page.find(ibss.name) != std::string::npos) {
    for(int i = 0; i < 3 && getline(keys, iBSSiv.page); i++) {
     if(iBSSiv.page.find("-iv") != std::string::npos){
      for(int i = 0; i < iBSSiv.page.size(); i++) {
       if(iBSSiv.page[i] == '"') {
        iBSSiv.page.erase(0, i);
         for(int u = 0; u < iBSSiv.page.size(); u++) {
          if(iBSSiv.page[u] == '>') {
           iBSSiv.page.erase(0, u); iBSSiv.page.erase(0, 1);
            for(int y = 0; y < iBSSiv.page.size(); y++) {
             if(iBSSiv.page[y] == '<') {
              iBSSiv.page.erase(y); iBSSiv.iv = iBSSiv.page;
           }
          }
         }
        }
       }
      }
     }
    }
   }
  }
return iBSSiv.iv;
}

std::string iBSSIv() {

keys iBSSiv;

chdir((std::string("WD_") + identifier + "_" + version).c_str());
 std::ifstream key("keys.html");
  while(getline(key, iBSSiv.page)) { // stores everything from instream to std::string
   if(iBSSiv.page.find("| file   = iBSS") != std::string::npos) {
    for(int i = 0; i < 3 && getline(key, iBSSiv.page); i++) {
     if(iBSSiv.page.find("iv") != std::string::npos) {
      iBSSiv.page.erase(0, 11);
       iBSSiv.iv = iBSSiv.page;
     }
    }
   }
  }
  return iBSSiv.iv;
}

std::string iBSSKey() {

keys iBSSkey;

chdir((std::string("WD_") + identifier + "_" + version).c_str());
 std::ifstream key("keys.html");
  while(getline(key, iBSSkey.page)) { // stores everything from instream to std::string
   if(iBSSkey.page.find("| file   = iBSS") != std::string::npos) {
    for(int i = 0; i < 3 && getline(key, iBSSkey.page); ++i) {
     if(iBSSkey.page.find("key") != std::string::npos) {
      iBSSkey.page.erase(0, 11);
       iBSSkey.key = iBSSkey.page;
     }
    }
   }
  else {
    return iBSSkey.key;
  }
  }
  return iBSSkey.key;
}

std::string iBSSKEY() {

keys iBSSkey;

chdir((std::string("WD_") + identifier + "_" + version).c_str());
 std::ifstream keys("keys.html");
  while(getline(keys, iBSSkey.page)) {
   if(iBSSkey.page.find(ibss.name) != std::string::npos) {
    for(int i = 0; i < 3 && getline(keys, iBSSkey.page); i++) {
     if(iBSSkey.page.find("-key") != std::string::npos){
      for(int i = 0; i < iBSSkey.page.size(); i++) {
       if(iBSSkey.page[i] == '"') {
        iBSSkey.page.erase(0, i);
         for(int u = 0; u < iBSSkey.page.size(); u++) {
          if(iBSSkey.page[u] == '>') {
           iBSSkey.page.erase(0, u); iBSSkey.page.erase(0, 1);
            for(int y = 0; y < iBSSkey.page.size(); y++) {
             if(iBSSkey.page[y] == '<') {
              iBSSkey.page.erase(y); iBSSkey.key = iBSSkey.page;
           }
          }
         }
        }
       }
      }
     }
    }
   }
  }
return iBSSkey.key;
}

std::string iBECIV() {

keys iBECiv;

chdir((std::string("WD_") + identifier + "_" + version).c_str());
 std::ifstream keys("keys.html");
  while(getline(keys, iBECiv.page)) {
   if(iBECiv.page.find(ibec.name) != std::string::npos) {
    for(int i = 0; i < 3 && getline(keys, iBECiv.page); i++) {
     if(iBECiv.page.find("-iv") != std::string::npos){
      for(int i = 0; i < iBECiv.page.size(); i++) {
       if(iBECiv.page[i] == '"') {
        iBECiv.page.erase(0, i);
         for(int u = 0; u < iBECiv.page.size(); u++) {
          if(iBECiv.page[u] == '>') {
           iBECiv.page.erase(0, u); iBECiv.page.erase(0, 1);
            for(int y = 0; y < iBECiv.page.size(); y++) {
             if(iBECiv.page[y] == '<') {
              iBECiv.page.erase(y); iBECiv.iv = iBECiv.page;
           }
          }
         }
        }
       }
      }
     }
    }
   }
  }
return iBECiv.iv;
}

std::string iBECKEY() {

keys iBECkey;

chdir((std::string("WD_") + identifier + "_" + version).c_str());
 std::ifstream keys("keys.html");
  while(getline(keys, iBECkey.page)) {
   if(iBECkey.page.find(ibec.name) != std::string::npos) {
    for(int i = 0; i < 3 && getline(keys, iBECkey.page); i++) {
     if(iBECkey.page.find("-key") != std::string::npos){
      for(int i = 0; i < iBECkey.page.size(); i++) {
       if(iBECkey.page[i] == '"') {
        iBECkey.page.erase(0, i);
         for(int u = 0; u < iBECkey.page.size(); u++) {
          if(iBECkey.page[u] == '>') {
           iBECkey.page.erase(0, u); iBECkey.page.erase(0, 1);
            for(int y = 0; y < iBECkey.page.size(); y++) {
             if(iBECkey.page[y] == '<') {
              iBECkey.page.erase(y); iBECkey.key = iBECkey.page;
           }
          }
         }
        }
       }
      }
     }
    }
   }
  }
return iBECkey.key;
}

int Help() {
  std::cout << "Usage:" << std::endl;
  std::cout << "\t-d device identifier e.g. iPad7,5"        << std::endl;
  std::cout << "\t-i iOS version e.g. 14.8"                 << std::endl;
  std::cout << "\t-b boardconfig e.g. j71bAP"               << std::endl;
  std::cout << "\t-s SHSH Blob file"                        << std::endl;
  std::cout << "\t-p Pwn the device and remove sigchecks"   << std::endl;
  std::cout << "\t-v Just boot the device verbose"          << std::endl;
  std::cout << "\t-c Boot the device with custom boot logo" << std::endl;
  return 0;
}

void Cleaner(std::string identifier, std::string version) {
chdir((std::string("WD_") + identifier + "_" + version).c_str());
system("rm BuildManifest.plist && rm info.json && rm keys.html");
system("rm ibss.raw && rm ibss.pwn");
system("rm ibec.raw && rm ibec.pwn");
system("rm kernel.raw && rm kernel.im4p && rm kernel.patched");
}

int Ramdisk(std::string version) {
  std::ifstream iBSS((std::string("Patched") + "_" + identifier + "_" + version + "/iBSS.img4").c_str());
  if(!iBSS){
    std::cerr << "Something really went wrong. Exiting..." << std::endl;
    exit(1);
  }
  chdir((std::string("Patched") + "_" + identifier + "_" + version).c_str());
  sleep(2);
  std::cout << RED << Time() << RESET << " Sending iBSS.." << std::endl;
  system("irecovery -f iBSS.img4");
  sleep(1);
  system("irecovery -f iBSS.img4");
  std::cout << RED << Time() << RESET << " Done!" << std::endl << std::endl;
  sleep(2);
  std::cout << RED << Time() << RESET << " Sending iBEC.." << std::endl;
  system("irecovery -f iBEC.img4");
  system("irecovery -f iBEC.img4");
  std::cout << RED << Time() << RESET << " Done!" << std::endl << std::endl;
  sleep(2);
  system("irecovery -c \"go\"");
  sleep(2);
  std::cout << RED << Time() << RESET << " Sending Ramdisk.." << std::endl;
  system("irecovery -f ramdisk");
  std::cout << RED << Time() << RESET << " Done!" << std::endl << std::endl;
  sleep(2);
  system("irecovery -c \"ramdisk\"");
  sleep(2);
  std::cout << RED << Time() << RESET << " Sending DeviceTree.." << std::endl;
  system("irecovery -f DeviceTree.img4");
  std::cout << RED << Time() << RESET << " Done!" << std::endl << std::endl;
  sleep(2);
  system("irecovery -c \"devicetree\"");
  sleep(2);
  std::cout << RED << Time() << RESET << " Sending TrustCache.." << std::endl;
  system("irecovery -f Trustcache.img4");
  std::cout << RED << Time() << RESET << " Done!" << std::endl << std::endl;
  sleep(2);
  system("irecovery -c \"firmware\"");
  sleep(2);
  std::cout << RED << Time() << RESET << " Sending Kernelcache.." << std::endl;
  system("irecovery -f KernelCache.img4");
  std::cout << RED << Time() << RESET << " Done!" << std::endl;
  sleep(2);
  system("irecovery -c \"bootx\"");

  std::cout << "The device should now be booting in to the restoreramdisk. Now just run iproxy 2222 44 and open a new tab and type 'ssh root@localhost -p 2222' :)" << std::endl;
  return 0;
  }

void mount() {

  FILE* rd = popen("hdiutil attach ramdisk.dmg", "r");
  char path[200];

  while(fgets(path, sizeof(path)-22, rd) != NULL) {
   ipsw.rdpath = path;
    pclose(rd);
     ipsw.rdpath.erase(std::remove(ipsw.rdpath.begin(), ipsw.rdpath.end(), '\n'), ipsw.rdpath.end());
      ipsw.rdpath.erase(0, 15);
       ipsw.rdpath.erase(0, ' ');
        ipsw.rdpath.erase(0, 6);

  }
}
std::string tolower(std::string Boardid){
 std::for_each(Boardid.begin(), Boardid.end(), [](char & conv) {
   conv = ::tolower(conv);
 });
 return Boardid;
}

int Pwndevice() {

std::string chipid;

std::cout << "[?] What is your cpid? ";
std::cin >> chipid;

while(1) {
  if(chipid == "A10X" || chipid == "0x8011" || chipid == "t8011" || chipid == "S5L8960" || chipid == "A7" || chipid == "S5L8965"){
    std::ifstream t8010("ipwndfu_public");
    if(t8010) {
      chdir("ipwndfu_public");
      std::cout << "[!] Keep in mind. The device needs to be in DFU Mode!!" << '\n';
      system("clear");
      std::cout << "[i] Putting device in pwned DFU Mode..." << '\n';
      system("sudo ./ipwndfu -p");
      sleep(2);
      std::cout << "[i] Removing signature checks..." << '\n';
      system("sudo python rmsigchks.py");
      chdir("..");
      return 0;
      break;
    }
    else {
    system("git clone https://github.com/MatthewPierson/ipwndfu_public");
    chdir("ipwndfu_public");
    std::cout << "[!] Keep in mind. The device needs to be in DFU Mode!!" << '\n';
    system("clear");
    std::cout << "[i] Putting device in pwned DFU Mode..." << '\n';
    system("sudo ./ipwndfu -p");
    sleep(2);
    std::cout << "[i] Removing signature checks..." << '\n';
    system("sudo python rmsigchks.py");
    chdir("..");
    break;
    return 0;
  }
}
  else if(chipid == "A11" || chipid == "t8015" || chipid == "0x8015") {
    std::ifstream chkexploit("ipwndfu-8015");
    if(chkexploit) {
      chdir("ipwndfu-8015");
      std::cout << "[i] Putting device in pwned DFU Mode..." << '\n';
      system("sudo ./ipwndfu -p");
      sleep(2);
      std::cout << "[i] Removing signature checks..." << '\n';
      system("sudo ./ipwndfu --patch");
      chdir("..");
      return 0;
      break;
    }
    else {
    system("git clone https://github.com/m1stadev/ipwndfu-8015");
    chdir("ipwndfu-8015");
    std::cout << "[!] Keep in mind. The device needs to be in DFU Mode!!" << '\n';
    system("clear");
    std::cout << "[i] Putting device in pwned DFU Mode..." << '\n';
    system("sudo ./ipwndfu -p");
    sleep(2);
    std::cout << "[i] Removing signature checks..." << '\n';
    system("sudo ./ipwndfu --patch");
    chdir("..");
    break;
    return 0;
  }
}
  else if(chipid == "A10" || chipid == "0x8010" || chipid == "t8010" || chipid == "A9" || chipid == "s8000" || chipid == "S8000" || chipid == "S8003" || chipid == "s8003" || chipid == "S5L8950" || chipid == "A6") {
    std::ifstream ck("ipwnder_lite");
    if(ck) {
      chdir("ipwnder_lite");
      std::cout << "[i] Putting device in pwned dfu mode and removing sig checks..." << '\n';
      system("./ipwnder_macosx -p");
      chdir("..");
      return 0;
      break;
    }
    else {
    system("git clone https://github.com/dora2-iOS/ipwnder_lite.git");
    chdir("ipwnder_lite");
    std::cout << "[!] Keep in mind. The device needs to be in DFU Mode!!" << '\n';
    system("clear");
    std::cout << "[i] Putting device in pwned dfu mode and removing sig checks..." << '\n';
    system("make");
    system("./ipwnder_macosx -p");
    chdir("..");
    break;
    return 0;
  }
}
  else if(chipid == "A8" || chipid == "A8X" || chipid == "T7000" || chipid == "T7001" || chipid == "t7000" || chipid == "t7001" || chipid == "A8x") {
    std::ifstream eclipsa("eclipsa");
    if(eclipsa) {
      chdir("eclipsa");
      system("sudo ./eclipsa");
      chdir("..");
      return 0;
      break;
    }
    system("git clone https://github.com/0x7ff/eclipsa.git");
    chdir("eclipsa");
    system("make && ./eclipsa");
    chdir("..");
    return 0;
    break;
  }
  else {
    std::cout << "[?] The what now?? '" << chipid << "'???" << '\n';
    exit(EXIT_FAILURE); // if you specified -p after -d and -i the whole code will not run IF it ever hits this else statement
    break;
  }
 }
 return 0;
}
int VerboseBoot(std::string version) {
  std::ifstream iBSS((std::string("Patched") + "_" + identifier + "_" + version + "/iBSS.img4").c_str());
  if(!iBSS){
    std::cerr << "Something really went wrong. Exiting..." << std::endl;
    exit(1);
  }
  chdir((std::string("Patched") + "_" + identifier + "_" + version).c_str());
  sleep(2);
  std::cout << RED << Time() << RESET << " Sending iBSS.." << std::endl;
  system("irecovery -f iBSS.img4");
  sleep(1);
  system("irecovery -f iBSS.img4");
  std::cout << RED << Time() << RESET << " Done!" << std::endl << std::endl;
  sleep(2);
  std::cout << RED << Time() << RESET << " Sending iBEC.." << std::endl;
  system("irecovery -f iBEC.img4");
  system("irecovery -f iBEC.img4");
  std::cout << RED << Time() << RESET << " Done!" << std::endl << std::endl;
  sleep(2);
  system("irecovery -c \"go\"");
  sleep(2);
  std::cout << RED << Time() << RESET << " Sending DeviceTree.." << std::endl;
  system("irecovery -f DeviceTree.img4");
  std::cout << RED << Time() << RESET << " Done!" << std::endl << std::endl;
  sleep(2);
  system("irecovery -c \"devicetree\"");
  sleep(2);
  std::cout << RED << Time() << RESET << " Sending Kernelcache.." << std::endl;
  system("irecovery -f KernelCache.img4");
  std::cout << RED << Time() << RESET << " Done!" << std::endl;
  sleep(2);
  system("irecovery -c \"bootx\"");
  sleep(1);
  std::cout << "[i] Done! The device should be booting verbose :)" << '\n';
  return 0;
}
int BootWithCustomLogo(std::string version) {
  std::ifstream iBSS((std::string("Patched") + "_" + identifier + "_" + version + "/iBSS.img4").c_str());
  if(!iBSS){
    std::cerr << "Something really went wrong. Exiting..." << std::endl;
    exit(1);
  }
  chdir((std::string("Patched") + "_" + identifier + "_" + version).c_str());
  sleep(2);
  std::cout << RED << Time() << RESET << " Sending iBSS.." << std::endl;
  system("irecovery -f iBSS.img4");
  system("irecovery -f iBSS.img4");
  std::cout << RED << Time() << RESET << " Done!" << std::endl << std::endl;
  sleep(2);
  std::cout << RED << Time() << RESET << " Sending iBEC.." << std::endl;
  system("irecovery -f iBEC.img4");
  system("irecovery -f iBEC.img4");
  std::cout << RED << Time() << RESET << " Done!" << std::endl << std::endl;
  sleep(2);
  system("irecovery -c \"go\"");
  sleep(2);
  std::cout << RED << Time() << " Sending custom logo.." << RESET << std::endl;
  system("irecovery -f logo.img4");
  std::cout << RED << Time() << " Done!" << std::endl << std::endl;
  system("irecovery -c \"setpicture 5\"");
  std::cout << RED << Time() << RESET << " Sending DeviceTree.." << std::endl;
  system("irecovery -f DeviceTree.img4");
  std::cout << RED << Time() << RESET << " Done!" << std::endl << std::endl;
  sleep(2);
  system("irecovery -c \"devicetree\"");
  sleep(2);
  std::cout << RED << Time() << RESET << " Sending Kernelcache.." << std::endl;
  system("irecovery -f KernelCache.img4");
  std::cout << RED << Time() << RESET << " Done!" << std::endl;
  sleep(2);
  system("irecovery -c \"bootx\"");
  sleep(1);
  std::cout << "[i] Done! The device should be booting with a custom bootlogo! :)" << '\n';
  return 0;
}
};
#endif /* Components_hpp */
