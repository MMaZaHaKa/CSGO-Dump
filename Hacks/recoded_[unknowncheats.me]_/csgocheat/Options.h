#pragma once

struct ColorP
{
public:
	const char* Name;
	float* Ccolor;

	ColorP(const char* name, float* color)
	{
		this->Name = name;
		this->Ccolor = color;
	}
};

class cOptions
{
public:
	struct
	{
		struct
		{
			bool bAimbotEnable;
			bool bAutoShoot;
			bool bAutoScope;
			bool bAutoRevolver;
			bool bSilentAim;
			bool bAutoPistol;
			int  iHitbox;
			int  iSelection;
			bool AutoZeus;
			bool LegitBacktrack;
			bool Backtrack;
			bool RageBacktrack;
			bool ResolverTest;
			int Player[64];
			bool PlaylistActive;
		}Aimbot;
		struct
		{
			bool bVelocityPrediction;
			int	 iMultibox;
			bool bMultipoint;
			int  iPointscale;
			int  BoxR;
			int  BoxG;
			int  BoxB;
			int  AmmoR;
			int  AmmoG;
			int  AmmoB;
			int  DropR;
			int  DropG;
			int  DropB;
			int  bombr;
			int  bombg;
			int  bombb;
			int  modelr;
			int  modelg;
			int  modelb;
			int  modelr1;
			int  modelg1;
			int  modelb1;
			int  handr;
			int  handg;
			int  handb;
			int  iHitchance;
			int  iMinDmg;
			bool bAcceptKillDmg;
			bool bNoRecoil;
			bool bAutoStop;
			int iAutoStopKey;
		}Accuracy;
		struct
		{
			int  iAntiAimResolver;
			int  iAccuracyBoost;
			int  iPreferBaim;
			bool bPositionAdj;
			bool bBruteforce;
			bool bResolver;
			bool bResolver2;
			bool delta;
			bool delta2;
			bool ForceAngles;

		}HvH;	
		struct
		{
			struct 
			{
				bool bEnableAntiAim;
				bool bAtTargets;

				int  iPitch;
				int  iYaw;
				int  iYawAdd;
				int  iFakeYaw;
				int  iFakeYawAdd;
				bool lockside;
				int lockside2;
				bool Dynamic;
				bool FreeStanding;
				int LBYDELTA;

			}Stand;
			struct
			{
				bool bEnableAntiAim;
				bool bAtTargets;
				bool bMemeWalk;

				int  iPitch;
				int  iYaw;
				int  iYawAdd;
				int  iFakeYaw;
				int  iFakeYawAdd;
				bool lockside3;
				bool lockside3b;
				int lockside4;
			}Move;
			struct
			{
				bool bEnableEdge;

				int  iPitch;
				int  iYaw;
				int  iYawAdd;
				int  iFakeYaw;
				int  iFakeYawAdd;
			}Edge;
			struct
			{
				bool bFakeWalk;
				int  iFakeWalkKey;

			}Misc;
		}AntiAim;

	}Ragebot;
	struct
	{
		struct
		{
			struct
			{
				bool bEnableAimbot;
				bool bSilentAim;
				bool bAutoPistol;
				bool bOnKey;
				int  iKey;
				float  iAimbotFov;
				float  flAimbotRecoil;
				int  iAimbotHitbox;
			}Pistols;
			struct
			{
				bool bEnableAimbot;
				bool bSilentAim;
				bool bOnKey;
				int  iKey;
				float  iAimbotFov;
				float  flAimbotRecoil;
				int  iAimbotHitbox;
			}Rifles;
			struct
			{
				bool bEnableAimbot;
				bool bSilentAim;
				bool bOnKey;
				int  iKey;
				float  iAimbotFov;
				float  flAimbotRecoil;
				int  iAimbotHitbox;
			}Snipers;
			struct
			{
				bool bEnableAimbot;
				bool bSilentAim;
				bool bOnKey;
				int  iKey;
				float  iAimbotFov;
				float  flAimbotRecoil;
				int  iAimbotHitbox;
			}Shotguns;
			struct
			{
				bool bEnableAimbot;
				bool bSilentAim;
				bool bOnKey;
				int  iKey;
				float  iAimbotFov;
				float  flAimbotRecoil;
				int  iAimbotHitbox;
			}Smgs;
			struct
			{
				bool bEnableAimbot;
				bool bSilentAim;
				bool bOnKey;
				int  iKey;
				float  iAimbotFov;
				float  flAimbotRecoil;
				int  iAimbotHitbox;
			}Heavy;

		}Aimbot;
		struct
		{
			struct
			{
				bool bEnableTrigger;
				bool bTriggerOnKey;
				int  iTriggerKey;
			//	int  iTriggerDelay = 10;
				bool bTriggerRecoil;
				bool bAutoRevolver;
				bool bRevolverSecondary;
				bool bAutoPistol;

				bool bTriggerFullBody;
				bool bTriggerHead;
				bool bTriggerChest;
				bool bTriggerStomach;
				bool bTriggerArms;
				bool bTriggerLegs;
			}Pistols;
			struct
			{
				bool bEnableTrigger;
				bool bTriggerOnKey;
				int  iTriggerKey;
				//int  iTriggerDelay = 10;
				bool bTriggerRecoil;

				bool bTriggerFullBody;
				bool bTriggerHead;
				bool bTriggerChest;
				bool bTriggerStomach;
				bool bTriggerArms;
				bool bTriggerLegs;
			}Rifles;
			struct
			{
				bool bEnableTrigger;
				bool bTriggerOnKey;
				int  iTriggerKey;
				//int  iTriggerDelay = 10;
				bool bTriggerRecoil;

				bool bTriggerFullBody;
				bool bTriggerHead;
				bool bTriggerChest;
				bool bTriggerStomach;
				bool bTriggerArms;
				bool bTriggerLegs;
				bool bScoped;
			}Snipers;
			struct
			{
				bool bEnableTrigger;
				bool bTriggerOnKey;
				int  iTriggerKey;
				//int  iTriggerDelay = 10;
				bool bTriggerRecoil;

				bool bTriggerFullBody;
				bool bTriggerHead;
				bool bTriggerChest;
				bool bTriggerStomach;
				bool bTriggerArms;
				bool bTriggerLegs;
			}Shotguns;
			struct
			{
				bool bEnableTrigger;
				bool bTriggerOnKey;
				int  iTriggerKey;
				//int  iTriggerDelay = 10;
				bool bTriggerRecoil;

				bool bTriggerFullBody;
				bool bTriggerHead;
				bool bTriggerChest;
				bool bTriggerStomach;
				bool bTriggerArms;
				bool bTriggerLegs;
			}Smgs;
			struct
			{
				bool bEnableTrigger;
				bool bTriggerOnKey;
				int  iTriggerKey;
				//int  iTriggerDelay = 10;
				bool bTriggerRecoil;

				bool bTriggerFullBody;
				bool bTriggerHead;
				bool bTriggerChest;
				bool bTriggerStomach;
				bool bTriggerArms;
				bool bTriggerLegs;
			}Heavy;

		}Trigger;
	}Legitbot;
	struct
	{
		struct
		{
			struct
			{
				bool bBox;
				bool bColorBox;
				bool bHealthbar;
				bool SelfESP;
				bool bHealthText;
				bool bArmorBar;
				int	 iBarPos;
				bool bSkeleton;
				bool bAimlines;
				bool bPitch;
				bool bYaw;
				bool locksidearrows;
				float MEME[3] = { 0, 1.f, 0 };
				float MEME1[3] = { 0, 1.f, 0 };
				float MEME2[3] = { 0, 1.f, 0 };
				float MEME3[3] = { 0, 1.f, 0 };
				float MEME4[3] = { 0, 1.f, 0 };
				float MEME5[3] = { 0, 1.f, 0 };
				float MEME6[3] = { 0, 1.f, 0 };
				bool allweapons;
				bool Glowmeme;
				bool Bullet;
				bool LBYLINES;
				bool DmgIndector;
				bool Speclist;
				bool BackTrackTicks;
				bool Out;
				bool Radar;
				bool NasaWalk;
				bool NoSmoke;
				bool BacktrackChams;
				bool DrawMultiPoints;
				bool Glowmeme2;
			}EspBox;
			struct
			{
				bool bWeapons;
				bool  WeaponW;
				bool bAmmo;
				bool bName;
				bool bIsScoped;
				bool bIsFlashed;
				bool Money;
			}EspMisc;
			struct
			{
				bool bDroppedWeapons;
				bool bEnemyOnly;
				bool bXQZ;
				bool bC4;
				bool bHostage;
				bool Nade;
			}EspFilter;
		}Esp;
		struct
		{
			bool bInfoBox;
			bool Inaccuracy;
			bool Spreadcheck;

			bool bInfoLby;
			bool InfoECHO;
			bool bInfoVelocity;
			bool LC;
			bool bClantag;
			bool bInfoAngles;
			bool bPlayerBox;
			bool Stats;
			bool flAlphaFade;
			bool Grande;
		}Infobox;
		struct
		{
			bool bSpecList;

			bool bCrosshair;
			bool bNormalCrosshair;
			bool bRecoilCrosshair;
			bool bSpreadCrosshair;
			bool bNoVisualRecoil;
			bool bRankReveal;
			bool bThirdperson;
			int  iThirdPerson;
			int  iThirdPersonKey;
			float flThirdPersonRange;
			bool bScopeRemover;
			bool bEngineRadar;
			bool bWatermark = true;
			bool bTime;
			bool bHitMarker;
			int	 iHitsound;
			int  iKillsound;
			bool bBackTrackDot;
		}VisualsMisc;
		struct 
		{
			bool bAsusWalls;
			float flAsusFactor = 100;
			bool bNightMode;		
			bool bNoFlash;
			bool bNoSmoke;
			int	 iSkyboxChanger;
			bool bBulletTracer;
			float flTracerLenght = 1;

		}VisualsWorlds;
		struct
		{
			bool bPlayers;
			bool bEnemyOnly;
			bool bWireframe;
			bool bFlat;
			bool bThroughWalls;
			bool bTransparent;
			bool bArms;
			bool bArmsWireframe;
			bool bArmsFlat;
			bool bArmsThroughWalls;
			bool bArmsTransparent;
			bool Weapon;
			bool bWframe;
			bool bWFlat;
			bool bWThroughWalls;
			bool bWTransparent;
			bool bFakeAngleChams;
		}Chams;
		struct 
		{
			bool bEnabled;
			bool bEnemyOnly;
			bool bPlayers;
			bool bChickens;
			bool bC4Carrier;
			bool bPlantedC4;
			bool bDefuseKits;
			bool bWeapons;
		}Glow;
	}Visuals;
	struct
	{
		struct
		{
			int  iFakeLag;
			bool bInAirOnly;
			int  iFakeLagFactor;
			//bool bAutoAccpet;
			bool bClanTag;
			bool bCorruption;
			int  iClanTagSelection;
			bool bClanTagAnimation;
			bool bFreeCam;
			int  iFreeCamKey;
			float flFreeCamSpeed = 5;
			bool Skin;
			int SkinAk;
		}Misc1;
		struct
		{
			bool bAntiUntrusted = true;
			bool bChatSpam;
			int  iNameSpam;
			bool bKnifeBot;
			bool bBlockBot;
			int  iBlockBotKey;
			bool bEnableFov;
			float flFovValue = 90;
			float	flViewModelValue;
			bool Autobuy;
				int iAutoBuy;
				bool buylist;
		}Misc2;
		struct
		{
			bool bAutoJump;
			bool bAutoStrafe;
			bool bAirStuck;
			bool bSlowmotion;
			bool bZStrafe;
			bool bCircleStrafer;
			bool bWalkBot;
			bool bBunnyhop;
			float flDistance = 0;
			int  iTurn = 0;
			int  iAirStuck;
			int  iAirStuckKey;
			int  iSlowMotionKey;
			int  iZStrafeKey;
			int  iCircleStraferMode;
			int  iCircleStraferKey;
			int  iAutoStrafeMode;
			int  Configsss;


		}Movement;
	}Misc;
	struct 
	{

		
	}Col;
	struct
	{
		bool NoRecoil;
		bool GloveOver;
		int Knifemodels;
		int Glovemodel;
		int Gloveskin;
		bool bEnableSkins;
		bool Skinsids;
		bool startrak;
		int  WeaponModel;
		int WeaponModel1;
		int WeaponModel2;
		int WeaponModel3;
		int WeaponModel4;
		int WeaponModel5;
		int WeaponModel6;
		int WeaponModel7;
		int WeaponModel8;
		int WeaponModel9;
		int WeaponModel10;
		int WeaponModel11;
		int WeaponModel12;
		int WeaponModel13;
		int WeaponModel14;
		int WeaponModel15;
		int WeaponModel16;
		int WeaponModel17;
		int WeaponModel18;
		char ChangerName[32] = "";
		int ChangerSkin;
		int ChangerSkin1;
		int ChangerSkin2;
		int ChangerSkin3;
		int ChangerSkin4;
		int ChangerSkin5;
		int ChangerSkin6;
		int ChangerSkin7;
		int ChangerSkin8;
		int ChangerSkin9;
		int ChangerSkin10;
		int ChangerSkin11;
		int ChangerSkin12;
		int ChangerSkin13;
		int ChangerSkin14;
		int ChangerSkin15;
		int ChangerSkin16;
		int ChangerSkin17;
		int ChangerSkin18;
		int ChangerSkin19;
		int stat;
		bool CustomName;
		bool knife;
	}Skins;
	struct 
	{
		bool setup = false;
		int current;
	}Config;
};

extern cOptions Options;