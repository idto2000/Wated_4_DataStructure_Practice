#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <clocale>
#include <Windows.h>
#include <cstring>

#include "Container/Manager.h"

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	std::setlocale(LC_ALL, ".UTF8");

	Manager manager;
	char command[20];

    std::cout << "==================================================\n";
	std::cout << "       레이저 블랙위도우 TE 크로마 V2 시뮬레이터\n";
    std::cout << "==================================================\n";
    
    std::cout << " - 키보드 입력 : A, B, C, D, E\n";
    std::cout << " - 되돌리기    : undo\n";
    std::cout << " - 다시 실행   : redo\n";
    std::cout << " - 상태 확인   : show\n";
    std::cout << " - 종료        : exit\n";

    while (true) {
        std::cout << "--------------------------------------------------\n";
        std::cout << "명령어 입력 >> ";

        // 사용자가 띄어쓰기 전까지 입력한 단어를 문자 배열(command)에 저장.
        if (!(std::cin >> command)) break;

        // strcmp()두 문자열이 완벽히 똑같으면 0 반환.
        if (strcmp(command, "exit") == 0) {
            std::cout << "시뮬레이터를 종료합니다.\n\n";
            break;
        }
        else if (strcmp(command, "undo") == 0) {
            manager.Undo();
        }
        else if (strcmp(command, "redo") == 0) {
            manager.Redo();
        }
        else if (strcmp(command, "show") == 0) {
            manager.Show();
        }
        // 입력된 글자 수가 딱 1개(strlen)이고, 그 글자가 'A'~'E' 사이인 경우
        else if (strlen(command) == 1 && command[0] >= 'A' && command[0] <= 'E') {
            manager.OnKeyPress(command[0]);
        }
        else {
            std::cout << "\n[오류] 잘못된 명령어입니다.";
            std::cout << "(A~E, undo, redo, show, exit 중 하나를 입력하세요)\n";
        }
    }

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();

	std::cin.get();
	return 0;
}