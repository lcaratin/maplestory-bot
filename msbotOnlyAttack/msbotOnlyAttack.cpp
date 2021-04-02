// msBot.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <iostream>
#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <thread>
#include <functional>
#include <string>     // std::string, std::to_string

#include <iostream>
#include <chrono>
#include <ctime>

HWND hwnd;
boolean rightWalk = true;
double MAX_TELEPORT = 6;
double TELEPORT_COUNT = 0;

// IMPLEMENTAR POTTING

DWORD AttackKey = 0x21; // ATTACK_1 - VK_PRIOR - PAGE_UP
DWORD TeleportKey = 0x22; // TELEPORT_1 - VK_NEXT - PAGE_DOWN
DWORD HomeKey = 0x24; // BUFF_1 - VK_HOME - Home
DWORD EndKey = 0x23; // BUFF_2 - VK_END - End
DWORD DelKey = 0x2E; // BUFF_3 - VK_INSERT - Ins

DWORD ZKey = 0x5A; // PICK UP ITEM - Z 

DWORD RightArrowKey = 0x27; // VK_RIGHT
DWORD LeftArrowKey = 0x25; // VK_RIGHT

DWORD HPPotKey = 0x44;
DWORD MPPotKey = 0x46;

// LPARAMS

LPARAM AttackKey_lParam = (MapVirtualKey(AttackKey, 0) << 16) + 1;
LPARAM TeleportKey_lParam = (MapVirtualKey(TeleportKey, 0) << 16) + 1;
LPARAM HomeKey_lParam = (MapVirtualKey(HomeKey, 0) << 16) + 1;
LPARAM EndKey_lParam = (MapVirtualKey(EndKey, 0) << 16) + 1;
LPARAM DelKey_lParam = (MapVirtualKey(DelKey, 0) << 16) + 1;

LPARAM RightArrowKey_lParam = (MapVirtualKey(RightArrowKey, 0) << 16) + 1;
LPARAM LeftArrowKey_lParam = (MapVirtualKey(LeftArrowKey, 0) << 16) + 1;

LPARAM HPPotKey_lParam = (MapVirtualKey(HPPotKey, 0) << 16) + 1;
LPARAM MPPotKey_lParam = (MapVirtualKey(MPPotKey, 0) << 16) + 1;

LPARAM ZKey_lParam = (MapVirtualKey(ZKey, 0) << 16) + 1;

double BUFF_COOLDOWN = 30;
double WALK_COOLDOWN = 50;
double POT_COOLDOWN = 7; // era 120 de cooldown
double ATTACK_COOLDOWN = 3;

double CROSSMAP_COOLDOWN = 20;

time_t BUFF_TIMER;
time_t WALK_TIMER;
time_t POT_TIMER;
time_t ATTACK_TIMER;

time_t CROSSMAP_TIMER;

time_t NOW;

boolean BUFF_ACTIVATE = false;
boolean WALK_ACTIVATE = false;
boolean POT_ACTIVATE = true;
boolean ATTACK_ACTIVATE = true;

boolean CROSSMAP_ACTIVATE = true;

void timer_start(std::function<void(void)> func, unsigned int interval)
{
    std::thread([func, interval]() {
        while (true)
        {
            func();
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
        }).detach();
}

void PostKey(UINT operation, WPARAM key, LPARAM lParam) {
    PostMessage(hwnd, operation, key, lParam);
}

void ExecuteKey(WPARAM key, LPARAM lParam, DWORD sleep) {
    PostKey(WM_KEYDOWN, key, lParam);
    if (sleep != 0) Sleep(sleep);
    PostKey(WM_KEYUP, key, lParam);
}

// Execute Methods
void ExecuteAttack() {
    ExecuteKey(AttackKey, AttackKey_lParam, 0);
}

void PickUpItem() {
    ExecuteKey(ZKey, ZKey_lParam, 0);
}

void ExecuteBuff() {
    ExecuteKey(HomeKey, HomeKey_lParam, 0);
    Sleep(2500);
    ExecuteKey(EndKey, EndKey_lParam, 0);
    Sleep(2500);
    //ExecuteKey(DelKey, DelKey_lParam, 0);
}

void ExecutePot() {
    //ExecuteKey(HPPotKey, HPPotKey_lParam, 0);
    //ExecuteKey(MPPotKey, MPPotKey_lParam, 0);
    //ExecuteKey(MPPotKey, MPPotKey_lParam, 0);
    //ExecuteKey(MPPotKey, MPPotKey_lParam, 0);
    ExecuteKey(MPPotKey, MPPotKey_lParam, 0);
    //ExecuteKey(MPPotKey, MPPotKey_lParam, 0);
    //ExecuteKey(HPPotKey, HPPotKey_lParam, 0);
    //ExecuteKey(HPPotKey, HPPotKey_lParam, 0);

    // era 1 HP
    // era 6 MP

}

void ExecuteRightTeleport() {

    SetForegroundWindow(hwnd);

    keybd_event(VK_RIGHT, MapVirtualKey(VK_RIGHT, 0), KEYEVENTF_EXTENDEDKEY, 200);
    ExecuteKey(TeleportKey, TeleportKey_lParam, 0);
    Sleep(200);
    keybd_event(VK_RIGHT, MapVirtualKey(VK_RIGHT, 0), KEYEVENTF_KEYUP, 0);
}

void ExecuteLeftTeleport() {

    SetForegroundWindow(hwnd);

    keybd_event(VK_LEFT, MapVirtualKey(VK_LEFT, 0), KEYEVENTF_EXTENDEDKEY, 200);
    ExecuteKey(TeleportKey, TeleportKey_lParam, 0);
    Sleep(200);
    keybd_event(VK_LEFT, MapVirtualKey(VK_LEFT, 0), KEYEVENTF_KEYUP, 0);
}


void ExecuteRightWalk() {
    /*ExecuteKey(RightArrowKey, RightArrowKey_lParam, 0);
    PostMessage(hwnd, WM_SYSKEYDOWN, VK_RIGHT, 0);
    Sleep(300);
    PostMessage(hwnd, WM_SYSKEYUP, VK_RIGHT, 0);

    SetForegroundWindow(hwnd);

    keybd_event(VK_RIGHT, MapVirtualKey(VK_RIGHT, 0), KEYEVENTF_EXTENDEDKEY, 200);
    ExecuteKey(TeleportKey, TeleportKey_lParam, 0);
    Sleep(200);
    keybd_event(VK_RIGHT, MapVirtualKey(VK_RIGHT, 0), KEYEVENTF_KEYUP, 0);

    ExecuteAttack();*/

    ExecuteKey(RightArrowKey, RightArrowKey_lParam, 500);

    SetForegroundWindow(hwnd);

    keybd_event(VK_RIGHT, MapVirtualKey(VK_RIGHT, 0), KEYEVENTF_EXTENDEDKEY, 200);
    Sleep(200);
    keybd_event(VK_RIGHT, MapVirtualKey(VK_RIGHT, 0), KEYEVENTF_KEYUP, 0);
}

void ExecuteLeftWalk() {
    ExecuteKey(LeftArrowKey, LeftArrowKey_lParam, 500);

    SetForegroundWindow(hwnd);

    keybd_event(VK_LEFT, MapVirtualKey(VK_LEFT, 0), KEYEVENTF_EXTENDEDKEY, 200);
    Sleep(200);
    keybd_event(VK_LEFT, MapVirtualKey(VK_LEFT, 0), KEYEVENTF_KEYUP, 0);
}
// END Execute Methods

void BotFlow() {

    // Get NOW TimeStamp
    NOW = time(0);

    // Prepare Focus
    SetFocus(hwnd);
    SetActiveWindow(hwnd);

    if (difftime(NOW, BUFF_TIMER) >= BUFF_COOLDOWN) {
        std::cout << "execute buff\n";
        BUFF_ACTIVATE = true;

        Sleep(1000); // removido no agressive
        ExecuteBuff();
    }

    if (difftime(NOW, WALK_TIMER) >= WALK_COOLDOWN && WALK_ACTIVATE == false) {
        // execute walk
        std::cout << "execute walk\n";
        WALK_ACTIVATE = true;

        Sleep(1000);

        HWND currentWindow = GetForegroundWindow();
        if (rightWalk) {
            ExecuteRightWalk();
        }
        else {
            ExecuteLeftWalk();
        }
        SetForegroundWindow(currentWindow);

        rightWalk = !rightWalk;

        Sleep(500);
    }

    if (difftime(NOW, POT_TIMER) >= POT_COOLDOWN) {
        std::cout << "execute pot\n";
        POT_ACTIVATE = true;

        //Sleep(1000); // removido no agressive
        ExecutePot();
    }

    /*if (difftime(NOW, CROSSMAP_TIMER) >= CROSSMAP_COOLDOWN && CROSSMAP_ACTIVATE == false && TELEPORT_COUNT <= MAX_TELEPORT) {
        // execute walk
        std::cout << "execute teleport\n";
        CROSSMAP_ACTIVATE = true;

        Sleep(1000);

        HWND currentWindow = GetForegroundWindow();
        if (rightWalk) {
            ExecuteRightTeleport();
        }
        else {
            ExecuteLeftTeleport();
        }
        SetForegroundWindow(currentWindow);

        //rightWalk = !rightWalk;

        Sleep(500);

        TELEPORT_COUNT += 1;
    }*/
    
    if (BUFF_ACTIVATE) {
        BUFF_TIMER = time(0);
        BUFF_ACTIVATE = false;
    }


    if (WALK_ACTIVATE) {
        WALK_TIMER = time(0);
        WALK_ACTIVATE = false;
    }


    if (POT_ACTIVATE) {
        POT_TIMER = time(0);
        POT_ACTIVATE = false;
    }

    /*if (CROSSMAP_ACTIVATE) {
        CROSSMAP_ACTIVATE = false;
        if (TELEPORT_COUNT >= MAX_TELEPORT) {
            CROSSMAP_TIMER = time(0);
            rightWalk = !rightWalk;
            TELEPORT_COUNT = 0;
        }
    }

    if (difftime(NOW, ATTACK_TIMER) >= ATTACK_COOLDOWN) {
        std::cout << "execute attack\n";
        ATTACK_ACTIVATE = true;

        //Sleep(1000); // removido no agressive
        ExecuteAttack();
    }



    if (ATTACK_ACTIVATE) {
        ATTACK_TIMER = time(0);
        ATTACK_ACTIVATE = false;
    }
    */

    
    


    // execute attack

    ExecuteAttack();
    //PickUpItem();
    //ExecutePot();
}

int main()
{
    std::cout << "msBot start!\n";

    BUFF_TIMER = time(0);
    WALK_TIMER = time(0);
    POT_TIMER = time(0);

    hwnd = FindWindow(_T("MapleStoryClass"), NULL);

    //DWORD keyWord = 0x44;
    //LPARAM lParam = (MapVirtualKey(keyWord, 0) << 16) + 1;
    //PostMessage(hwnd, WM_KEYDOWN, keyWord, lParam);

    //timer_start(BotFlow, 500);

    while (true) {
        BotFlow();
        Sleep(100);
    }

    //testFlow();

    //struct tm newtime, newtime2;


    //time_t now = time(0);
    //Sleep(5000);
    //time_t now2 = time(0);
    //localtime_s(&newtime, &now);
    //localtime_s(&newtime2, &now2);
    //int Month = 1 + newtime.tm_mon;

    //double diff = difftime(now2, now);

    //if (diff >= 5)
    //        std::cout << ">= 5";
    //else std::cout << "xd";


}

// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuraçãoff
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln
