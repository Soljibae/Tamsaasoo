#pragma once
namespace InGame
{
	enum EnemyType
	{
		MINION = 0,
		ARCHER = 1,
		DASHER = 2,
		TANKER = 3,
		BOMBER = 4,
	};

	static EnemyType GetNextEnemyType(EnemyType InEnemyType)
	{
		switch (InEnemyType)
		{
		case EnemyType::MINION : 
			return EnemyType::ARCHER;
		case EnemyType::ARCHER : 
			return EnemyType::DASHER;
		case EnemyType::DASHER:
			return EnemyType::TANKER;
		case EnemyType::TANKER:
			return BOMBER;
		}
		return EnemyType::MINION;
	}

	struct EnemyData
	{
	public:
		EnemyType Type;
		AEGfxTexture* Texture = nullptr;
		s32 Damage = 0;
		u32 Exp = 0;
		s32 Health = 0;
		AEVec2 DrawSize;
		f32 CollisionRadius = 0.f;
		f32 MovementSpeed = 0.f;
		s32 row, column;
		f32 FrameTime;
		f32 BulletSpeed = 0;
		AEVec2 ProjectileSize;
		virtual void Init()
		{

		}
		~EnemyData()
		{
			if (Texture)
			{
				AEGfxTextureUnload(Texture);
			}
		}
	};

	struct MinionData : EnemyData
	{
	public:
		virtual void Init() override
		{
			Type = EnemyType::MINION;
			Texture = AEGfxTextureLoad("Assets/Minion.png");
			Damage = 1;
			Exp = 1;
			Health = 3;
			AEVec2Set(&DrawSize, 40.f, 40.f);
			CollisionRadius = 20;
			MovementSpeed = 200;
		}
	};

	struct ArcherData : EnemyData
	{
	public:
		virtual void Init() override
		{
			Type = EnemyType::ARCHER;
			Texture = AEGfxTextureLoad("Assets/Archer.png");
			Damage = 1;
			Exp = 2;
			Health = 1;
			AEVec2Set(&DrawSize, 30.f, 30.f);
			CollisionRadius = 15;
			MovementSpeed = 100;
			BulletSpeed = 20;
			AEVec2Set(&ProjectileSize,10.f,10.f);
		}
	};
	struct DasherData : EnemyData
	{
	public:
		virtual void Init() override
		{
			Type = EnemyType::DASHER;
			Texture = AEGfxTextureLoad("Assets/Dasher.png");
			Damage = 1;
			Exp = 4;
			Health = 1;
			AEVec2Set(&DrawSize, 30.f, 30.f);
			CollisionRadius = 15.f;
			MovementSpeed = 100;
		}
	};
	struct TankerData : EnemyData
	{
	public:
		virtual void Init() override
		{
			Type = EnemyType::TANKER;
			Texture = AEGfxTextureLoad("Assets/Tanker.png");
			Damage = 1;
			Exp = 5;
			Health = 30;
			AEVec2Set(&DrawSize, 50.f, 50.f);
			CollisionRadius = 25;
			MovementSpeed = 50;
		}
	};
	struct BomberData : EnemyData
	{
	public:
		virtual void Init() override
		{
			Type = EnemyType::BOMBER;
			Texture = AEGfxTextureLoad("Assets/Bomber.png");
			Damage = 4;
			Exp = 5;
			Health = 3;
			AEVec2Set(&DrawSize, 50.f, 50.f);
			CollisionRadius = 25;
			MovementSpeed = 30;
		}
	};
}