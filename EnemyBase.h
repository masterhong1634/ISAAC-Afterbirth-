#pragma once

// 몬스터 속성 구조체
struct EnemyInfo
{
	image*	enemyShadowImage;	// 적 그림자 이미지
	image*	enemyImage;			// 적 전체 이미지
	RECT	enemyHeadRect;		// 적 머리 상자
	RECT	enemyBodyRect;		// 적 몸 상자
	RECT	enemyRect;			// 적 전체 렉트
	RECT	enemyFireRange;		// 적의 판정 범위 상자
	int		enemyNumber;		// 몬스터 넘버
	int		enemyHp;			// 적 체력
	int		enemyShotDelay;		// 공격 주기
	float	enemyShotSpeed;		// 공격속도
	float	enemyShotRange;		// 공격 사거리
	float	enemySpeed;			// 이동속도
	float	enemyX;				// 적 x좌표
	float	enemyY;				// 적 y좌표
};

class EnemyBase
{
protected:
	// 미니언 애니메이션
	animation* minionAni;

	// 보스 애니메이션
	animation* bossAni;

	// 몬스터의 정보
	EnemyInfo enemy;

	int firstEnemyAiTime;		// 첫번째 적 AI 행동시간
	int firstEnemyAiPattern;	// 첫번째 적 AI 행동패턴
	int firstEnemyBulletCount;	// 첫번째 적 불렛 인터벌 카운트
	int secondEnemyAiTime;		// 두번째 적 AI 행동시간
	int secondEnemyAiPattern;	// 두번째 적 AI 행동패턴
	int secondEnemyBulletCount;	// 두번째 적 불렛 인터벌 카운트
	int thirdEnemyAiTime;		// 세번째 적 AI 행동시간
	int thirdEnemyAiPattern;	// 세번째 적 AI 행동패턴
	int thirdEnemyBulletCount;	// 세번째 적 불렛 인터벌 카운트
	int i;						// for문용 변수

	float distance;				// 적과 플레이어의 거리
	float vx, vy;				// 접근 속도 변수

	bool enemyAreaCheck;		// 적의 판정 범위와 플레이어의 충돌을 체크하는 변수
	bool enemyAtk;				// 미니언 공격 체크
	bool enemyDeath;			// 적의 죽음을 체크해주는 변수

public:
	EnemyBase();
	~EnemyBase();

	// 생성할 때 포지션으로 자리를 잡아준다. position에 들어갈 변수는 PointMake(x, y)
	virtual HRESULT Init(POINT position);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	// 적의 불렛 카운트를 한 번에 플러스
	int SetFirstEnemyBulletCount() { firstEnemyBulletCount++; return firstEnemyBulletCount; }
	int SetSecondEnemyBulletCount() { secondEnemyBulletCount++; return secondEnemyBulletCount; }
	int SetThirdEnemyBulletCount() { thirdEnemyBulletCount++; return thirdEnemyBulletCount; }
};
