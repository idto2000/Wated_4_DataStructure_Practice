#pragma execution_character_set("utf-8")

#include <iostream>
#include <clocale>
#include <Windows.h>
#include <cstring>
#include <string>

// 실패 오류 값.
#define FAIL -1

struct Node
{
	// 요구사항 
	char* playerName;
	int score;

	// 이진트리 탐색에 필요한 메모리 주소를 가리키는 포인터.
	// 기존 index는 정수형 데이터만 비교했음
	// 실습에서는 정수+문자열이 포함되어 있어 Node 통합 주소를 따라가게 설계.
	// Node의 주소 이동 방향을 나타내기 위함.
	Node* left; // 낮은 위치로 이동
	Node* right; // 높은 위치로 이동
};

// 노드 생성
Node* CreateNode(const char* name, int score)
{
	Node* newNode = new Node;

	// name의 길이를 구하고 playerName에 메모리 할당. 
	size_t length = strlen(name);
	newNode->playerName = new char[length + 1];

	// 문자열 복사.
	strcpy_s(newNode->playerName, length + 1, name);

	newNode->score = score;
	newNode->left = nullptr;
	newNode->right = nullptr;

	return newNode;
}

// insert 재귀함수: 이름 및 점수를 묶어서 저장(node로 묶어서 저장)
Node* Insert(Node* node, const char* name, int score)
{
	// 종료 조건
	if (node == nullptr)
	{
		return CreateNode(name, score);
	}

	// 점수를 비교해 낮으면 왼쪽, 높으면 오른쪽으로 분리.
	if (score < node->score)
	{
		node->left = Insert(node->left, name, score);
	}
	else
	{
		node->right = Insert(node->right, name, score);
	}

	return node;
}

// delete 재귀함수: 이름으로 검색 및 삭제
Node* Delete(Node* node, const char* name)
{
	// 종료 조건
	if (node == nullptr)
	{
		return nullptr;
	}

	// 이름이 일치하는지 비교(0: 참, 0이 아닌 수: 거짓).
	// strcmp():문자열 비교함수
	if (strcmp(node->playerName, name) == 0)
	{
		// 임시 저장될 장소 초기화
		Node* temp = nullptr;

		// 왼쪽 또는 오른쪽에 있을 경우
		if (node->right == nullptr)
		{
			temp = node->left;

			// 메모리 삭제
			delete[] node->playerName;
			delete node;
			return temp;
		}
		else if (node->left == nullptr)
		{
			temp = node->right;

			// 메모리 삭제
			delete[] node->playerName;
			delete node;
			return temp;
		}
	}
	else
	{
		// 왼쪽 또는 오른쪽 탐색.
		node->left = Delete(node->left, name);
		node->right = Delete(node->right, name);
	}

	return node;
}

// find 재귀함수: 이름으로 검색 후 출력
Node* Find(Node* node, const char* name)
{
	// 종료 조건
	if (node == nullptr)
	{
		return nullptr;
	}

	// 이름이 같은지 비교(현재 == 연산자의 경우 메모리 주소만 비교함).
	// strcmp()를 통해 두개의 문자열을 서로 비교, 같은지, 누가 더 앞인지 판단.
	if (strcmp(node->playerName, name) == 0)
	{
		// 이름이 같은면 출력
		std::cout << "이름: " << node->playerName << "점수: "
			<< node->score << "찾음\n";
		return node;
	}

	// node의 왼쪽/ 오른쪽을 순회하며 일치하는지 검색.
	// 이진트리 규칙 왼쪽 부터 검색
	Node* res = Find(node->left, name);
	if( res != nullptr)
	{
		return res;
	}

	return Find(node->right, name);
}

// top 재귀함수: 점수 높은 순 k명 출력
void Top(Node* node, int& count, int k)
{
	// 종료 조건
	if (node == nullptr || count >= k)
	{
		return;
	}

	// 오른쪽 트리 순회(높은 점수 검색)
	Top(node->right, count, k);

	// 현재 노드의 정보 출력
	if (count < k)
	{
		std::cout << ++count << "." << node->playerName << "("
			<< node->score << ")\n";
	}

	// 왼쪽 트리 순회(현재 노드 보다 낮은 점수 검색)
	Top(node->left, count, k);
}

// 전체 노드의 개수를 파악하는 재귀함수
int CountNodes(Node* node)
{
	if (node == nullptr)
	{
		return 0;
	}

	return 1 + CountNodes(node->left) + CountNodes(node->right);
}

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	std::setlocale(LC_ALL, ".UTF8");

	Node* root = nullptr;

	//배열
	const char* names[] = { "Alice", "Bob", "Charlie", "David", "Eve", "Frank", "Grace",
		"Heidi", "Ivan", "Judy", "Kevin", "Leo", "Mallory", "Nina", "Oscar", "Peggy", "Quinn",
		"Rupert", "Sybil", "Trent" };// 상수 타입 문자열
	int scores[] = { 850, 720, 910, 450, 680, 330, 990, 510, 880, 760, 620,
		490, 950, 810, 270, 640, 730, 890, 550, 920 };

	// 배열 길이
	int dataSize = sizeof(scores) / sizeof(scores[0]);

	// Insert 함수 호출
	for (int i = 0; i < dataSize; ++i)
	{
		root = Insert(root, names[i], scores[i]);
	}

	// 배열 전체 출력
	std::cout << "=== 전체 DB 보기 === \n";
	for (int ix = 0; ix < dataSize; ++ix)
	{
		std::cout << "[" << ix + 1 << "] " << names[ix] 
			<< ":" << scores[ix] << "점\n";
	}

	int k = 0;
	int rankCount = 0;
	int currentTotal = CountNodes(root);

	std::cout << "=== 랭킹 조회 === \n";

	// 입력된 값 체크
	while (true)
	{
		// 보고싶은 랭킹 순위 입력
		std::cout << "보고 싶은 상위 인원수(k)를 입력하세요: ";
		std::cin >> k;

		// 입력값이 1에서 전체 개수 사이인지 확인
		if (k >= 1 && k <= currentTotal)
		{
			break;
		}

		std::cout << "\n[오류] 잘못된 입력 값입니다. 전체 데이터는 " << currentTotal 
			<<" 개 입니다.\n" << "1에서 " << currentTotal <<" 사이의 숫자를 입력해주세요.\n\n";
	}

	// 입력 값이 올바르면 출력
	std::cout << "\n[현재 상위" << k << "명 명단]\n";
			
	Top(root, rankCount, k);

	// 삭제하고 싶은 이름 입력 및 검색 후 삭제 그리고 랭킹 보기
	char target[100];// 이름을 담을 충분한 크기 확보
	Node* foundNode = nullptr;

	std::cout << "\n=== 테이터 삭제 및 정리 === \n";

	while (true)
	{
		std::cout << "삭제할 이름을 입력하세요: \n";
		std::cin >> target;

		foundNode = Find(root, target);
		if (foundNode != nullptr)
		{
			// 이름이 있을 경우
			std::cout << "[확인]" << foundNode->playerName << "님의 데이터가 존재합니다."
				<< foundNode->score <<"점\n";
			break;
		}
		else
		{
			// 이름이 없을 경우
			std::cout << "\n[오류]해당" << target << "존재하지 않습니다.\n";
		}
	}

	// Delete 함수를 사용해 노드 삭제
	root = Delete(root, target);

	// 삭제 후 삭제가 잘되었는지 한번 더 찾아본다. 
	if (Find(root, target) == nullptr)
	{
		std::cout << "'" << target << "' 데이터를 성공적으로 삭제했습니다.\n";
	}

	// 삭제 후 업데이트된 랭킹 보기
	std::cout << "\n[삭제 후 업데이트된 랭킹] [" << k <<"]\n";
	rankCount = 0;
	Top(root, rankCount, k);

	std::cin.get();	
	return 0;
}