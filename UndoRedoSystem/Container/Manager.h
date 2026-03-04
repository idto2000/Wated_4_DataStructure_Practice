#pragma once

#include "Container/Queue.h"
#include "Container/Stack.h"

class Manager
{
public:
	Manager();
	~Manager();

	// 키 입력 처리 (A, B, C, D, E)
	void OnKeyPress(char key);

	// 되돌리기(Undo) 실행
	void Undo();

	// 다시 실행(Redo) 실행
	void Redo();

	// 현재 시스템 상태 출력
	void Show();

private:
	// 새로운 입력 시 Redo 스택을 초기화하는 보조 함수
	void ClearRedoStack();

private:

	// 원본 템플릿 클래스를 사용하여 문자(char)를 담는 객체 생성
	// 입력(Queue)
	Queue<char, 10> inputQueue;   
	
	// 되돌리기(Undo) 스택
	Stack<char, 10> undoStack;    
	
	// 다시 실행(Redo) 스택
	Stack<char, 10> redoStack;    
};

