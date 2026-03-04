#include <iostream>

#include "Manager.h"

Manager::Manager()
{
}

Manager::~Manager()
{
}

void Manager::OnKeyPress(char key)
{
	if (inputQueue.Enqueue(key))
	{
		std::cout << "\n " << key << " 키가 Queue에 들어갔습니다.";

		// Queue에서 꺼내어 Undo스텍에 기록
		char data;
		if (inputQueue.Dequeue(data))
		{
			undoStack.Push(data);

			// 새로운 데이터가 입려되면 그 전 데이터 삭제 
			ClearRedoStack();
			std::cout << "\n "
				<< data << " 작업이 되돌리기(Undo) 스택에 기록되었습니다.\n";
		}
	}
	else
	{
		std::cout << "\n 대기열이 가득 차" << key 
			<< " 키를 입력받을 수 없습니다!\n";
	}
}

void Manager::Undo()
{
	char target;

	// Stack의 Pop 사용
	if (undoStack.Pop(target))
	{
		redoStack.Push(target);
		std::cout << "\n 되돌리기(Undo) 성공: "
			<< target << " 입력이 취소되었습니다.\n";
	}
	else
	{
		std::cout << "\n 되돌릴(Undo) 작업이 남아있지 않습니다.\n";
	}
}

void Manager::Redo()
{
	char target;

	// Stack의 Pop 사용
	if (redoStack.Pop(target)) {
		undoStack.Push(target);
		std::cout << "\n 다시 실행(Redo) 성공: " << target 
			<< " 입력이 복구되었습니다.\n";
	}
	else {
		std::cout << "\n 다시 실행할(Redo) 작업이 남아있지 않습니다.\n";
	}
}

void Manager::Show()
{
	std::cout << "\n=============== [현재 시스템 상태] ===============\n\n";

	// Stack의 Count() 함수 사용
	std::cout << " ▶ 되돌리기 (Undo) 스택: 총 " << undoStack.Count() << "개의 작업 기록됨\n";
	std::cout << " ▶ 다시 실행 (Redo) 스택: 총 " << redoStack.Count() << "개의 작업 기록됨\n";
	std::cout << "\n===================================================\n";
}

void Manager::ClearRedoStack()
{
	char temp;

	// Stack의 Clear()가 private이므로 Pop을 반복하여 안전하게 비움
	while (!redoStack.IsEmpty())
	{
		redoStack.Pop(temp);
	}
}
