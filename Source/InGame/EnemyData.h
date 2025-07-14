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
		ZIGZAG = 5,
		ORBITER = 6,
		SNIPER = 7,
		BURNER = 8,
		HOLER = 9,
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
		case EnemyType::BOMBER:
			return ZIGZAG;
		case EnemyType::ZIGZAG:
			return ORBITER;
		case EnemyType::ORBITER:
			return SNIPER;
		case EnemyType::SNIPER:
			return BURNER;
		}
		return EnemyType::MINION;
	}

	struct EnemyData
	{
	public:
		EnemyType Type;
		AEGfxTexture* Texture = nullptr;
		f32 Damage = 0.f;
		u32 Exp = 0;
		f32 Health = 0.f;
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
			Texture = AEGfxTextureLoad("Assets/Slime.png");
			Damage = 1;
			Exp = 1;
			Health = 1;
			AEVec2Set(&DrawSize, 50.f, 50.f);
			CollisionRadius = 25;
			MovementSpeed = 100;
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
			Health = 2;
			AEVec2Set(&DrawSize, 70.f, 70.f);
			CollisionRadius = 35.f;
			MovementSpeed = 100;
			BulletSpeed = 5;
			AEVec2Set(&ProjectileSize,40.f,40.f);
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
			Health = 3;
			AEVec2Set(&DrawSize, 100.f, 100.f);
			CollisionRadius = 25.f;
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
	struct ZigZagData : EnemyData
	{
	public:
		virtual void Init() override
		{
			Type = EnemyType::ZIGZAG;
			Texture = AEGfxTextureLoad("Assets/ZigZag.png");
			Damage = 1;
			Exp = 1;
			Health = 6;
			AEVec2Set(&DrawSize, 70.f, 70.f);
			CollisionRadius = 35;
			MovementSpeed = 200;
		}
	};
	struct OrbiterData : EnemyData
	{
	public:
		virtual void Init() override
		{
			Type = EnemyType::ORBITER;
			Texture = AEGfxTextureLoad("Assets/Orbiter.png");
			Damage = 1;
			Exp = 1;
			Health = 3;
			AEVec2Set(&DrawSize, 70.f, 70.f);
			CollisionRadius = 35;
			MovementSpeed = 200;
			BulletSpeed = 5;
			AEVec2Set(&ProjectileSize, 20.f, 20.f);
		}
	};
	struct SniperData : EnemyData
	{
	public:
		virtual void Init() override
		{
			Type = EnemyType::SNIPER;
			Texture = AEGfxTextureLoad("Assets/Sniper.png");
			Damage = 1;
			Exp = 2;
			Health = 10;
			AEVec2Set(&DrawSize, 70.f, 70.f);
			CollisionRadius = 35.f;
			MovementSpeed = 300;
			BulletSpeed = 5;
			AEVec2Set(&ProjectileSize, 20.f, 20.f);
		}
	};
	struct BurnerData : EnemyData
	{
	public:
		virtual void Init() override
		{
			Type = EnemyType::BURNER;
			Texture = AEGfxTextureLoad("Assets/Burner.png");
			Damage = 1;
			Exp = 2;
			Health = 50;
			AEVec2Set(&DrawSize, 100.f, 100.f);
			CollisionRadius = 50.f;
			MovementSpeed = 100;
		}
	};
	struct HolerData : EnemyData
	{
	public:
		virtual void Init() override
		{
			Type = EnemyType::HOLER;
			Texture = AEGfxTextureLoad("Assets/Holer.png");
			Damage = 1;
			Exp = 2;
			Health = 100;
			AEVec2Set(&DrawSize, 100.f, 100.f);
			CollisionRadius = 50.f;
			MovementSpeed = 100;
		}
	};
}