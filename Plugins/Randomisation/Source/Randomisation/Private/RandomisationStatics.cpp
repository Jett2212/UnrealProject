// Luke Jackson 09/08/24

#include "RandomisationStatics.h"
#include "Containers/Map.h"
#include "Misc/App.h"

float RandomisationStatics::FSeededRand ( const int32& InSeed, const int32& InMultiplyNum /*= 196314165*/, const int32& InAdditionNum /*= 907633515*/ )
{
	int32 GSRandSeed = (InSeed * InMultiplyNum) + InAdditionNum;
	union { float f; int32 i; } Result;
	union { float f; int32 i; } Temp;
	const float SRandTemp = 1.0f;
	Temp.f = SRandTemp;
	Result.i = (Temp.i & 0xff800000) | (GSRandSeed & 0x007fffff);

	return Result.f - 1.f;
}

double RandomisationStatics::DSeededRand(const int32& InSeed, const int32& InMultiplyNum /*= 196314165*/, const int32& InAdditionNum /*= 907633515*/)
{
	//int32 GSRandSeed = (InSeed * InMultiplyNum) + InAdditionNum ;//* (InSeed*InMultiplyNum % 51));
	//UE_LOG(LogTemp, Warning, TEXT("%f"), float(InSeed*InMultiplyNum % 51))
	
	/*
	union { float f; int32 i; } Result;
	union { float f; int32 i; } Temp;
	const float SRandTemp = 1.0f;
	Temp.f = SRandTemp;
	Result.i = (Temp.i & 0xff800000) | (GSRandSeed & 0x007fffff);

	*/

	union { float f; int32 i; } Temp;
	const float SRandTemp = 1.0f;
	Temp.f = SRandTemp;

	int32 GSRandSeed = (InSeed * InMultiplyNum) + InAdditionNum ;

	int32 Temp1Int = (InMultiplyNum * InSeed);
	int32 Temp2Int = (InAdditionNum + InSeed);

	union { float f; int32 i; } Test3;
	Test3.i = (Temp.i & 0xff800000) | (InMultiplyNum & 0x007fffff);

	union { float f; int32 i; } Test4;
	Test4.i = (Temp.i & 0xff800000) | (InAdditionNum & 0x007fffff);

	//Temp1Int *= InAdditionNum * InMultiplyNum * InSeed;
	//Temp2Int += InAdditionNum * InMultiplyNum;

	union { float f; int32 i; } Test1;
	Test1.i = (Temp.i & 0xff800000) | (Temp1Int & 0x007fffff);

	union { float f; int32 i; } Test2;
	Test2.i = (Temp.i & 0xff800000) | (Temp2Int & 0x007fffff);

	float TempFloat = Test1.f * Test2.f * Test3.f *	Test4.f;

	float Weight = TempFloat - FMath::TruncToFloat(TempFloat);

	float Final = Weight;
	/**/
	if (Weight <= 0.2)
	{
		Final = TempFloat * Weight;
	}
	else if (Weight <= 0.4)
	{
		Final = Weight;
	}
	else if (Weight <= 0.6)
	{
		Final = TempFloat * Test1.f;
	}
	else if (Weight <= 0.8)
	{
		Final = TempFloat * Test2.f;
	}
	else 
	{
		Final = Weight * Weight;
	}

	return  Final - FMath::TruncToFloat(Final); 


}

int32 RandomisationStatics::I32Rand ()
{
	// Use a cut of Time as a seed to generate a seed to output
	int64 Time = FMath::FloorToInt64( FApp::GetCurrentTime() * 100) ;
	int32 Seed { (Time & 0xffff) };

	int32 ReturnSeed { FMath::FloorToInt32(DSeededRand(Seed, 135468356, 786543721) * (Seed+1) * 100)};
	
	return ReturnSeed;
}

float RandomisationStatics::FSeededRandRange ( const int32& InSeed, const float& Min, const float& Max, const int32& InMultiplyNum /*= 196314165*/, const int32& InAdditionNum /*= 907633515*/ )
{
	return Min + (Max - Min) * FSeededRand(InSeed, InMultiplyNum, InAdditionNum);
}

double RandomisationStatics::DSeededRandRange ( const int32& InSeed, const double& Min, const double& Max, const int32& InMultiplyNum /*= 196314165*/, const int32& InAdditionNum /*= 907633515*/ )
{
	return Min + (Max - Min) * DSeededRand(InSeed, InMultiplyNum, InAdditionNum);
}


FVector RandomisationStatics::VecSeededRand ( const int32& InSeed, const bool& IgnoreZ, const int32& InMultiplyNum /*= 196314165*/, const int32& InAdditionNum /*= 907633515*/ )
{
	return FVector(0, 0, 0);
}

FVector RandomisationStatics::VecSeededRandRange ( const int32& InSeed, const double& Min, const double& Max, const bool& bIgnoreZ, const int32& InMultiplyNumX /*= 196314165*/, const int32& InAdditionNumX /*= 907633515*/, const int32& InMultiplyNumY /*= 456211287*/, const int32& InAdditionNumY /*= 762463905*/, const int32& InMultiplyNumZ /*= 606967995*/, const int32& InAdditionNumZ /*= 331255672*/ )
{
	double X {DSeededRandRange(InSeed, Min, Max, InMultiplyNumX, InAdditionNumX)};
	double Y {DSeededRandRange(InSeed, Min, Max, InMultiplyNumY, InAdditionNumY)};
	double Z {0.0};
	if (!bIgnoreZ)
	{
		Z = DSeededRandRange(InSeed, Min, Max, InMultiplyNumZ, InAdditionNumZ);
	}

	return FVector(X, Y, Z);
}


FVector RandomisationStatics::VecSeededRandRange ( const int32& InSeed, const double& MinX, const double& MaxX, const double& MinY, const double& MaxY, const double& MinZ, const double& MaxZ, const bool& bIgnoreZ, const int32& InMultiplyNumX /*= 196314165*/, const int32& InAdditionNumX /*= 907633515*/, const int32& InMultiplyNumY /*= 456211287*/, const int32& InAdditionNumY /*= 762463905*/, const int32& InMultiplyNumZ /*= 606967995*/, const int32& InAdditionNumZ /*= 331255672*/ )
{
	double X {DSeededRandRange(InSeed, MinX, MaxX, InMultiplyNumX, InAdditionNumX)};
	double Y {DSeededRandRange(InSeed, MinY, MaxY, InMultiplyNumY, InAdditionNumY)};
	double Z {0.0};
	if (!bIgnoreZ)
	{
		Z = DSeededRandRange(InSeed, MinZ, MaxZ, InMultiplyNumZ, InAdditionNumZ);
	}

	return FVector(X, Y, Z);
}

TArray<double> RandomisationStatics::DSeededRandRangeCount ( const int32& InSeed, const int32& Count, const double& Min, const double& Max, const int32& InMultiplyNum /*= 196314165*/, const int32& InAdditionNum /*= 907633515*/ )
{
	TArray<double> ReturnArr;
	int32 Seed { FMath::FloorToInt32(DSeededRand(InSeed, InMultiplyNum, InAdditionNum) * (InSeed+1) * 100)};
	ReturnArr.Init(0.0, Count);
	ParallelFor(Count, [&] (int32 Idx)
	{
		ReturnArr[Idx] = DSeededRandRange(Seed + Idx, Min, Max, InMultiplyNum, InAdditionNum);
	}, true);
	return ReturnArr;
}

TArray<FVector> RandomisationStatics::VecSeededRandRangeCount ( const int32& InSeed, const int32& Count, const double& Min, const double& Max, const bool& bIgnoreZ, const int32& InMultiplyNumX /*= 196314165*/, const int32& InAdditionNumX /*= 907633515*/, const int32& InMultiplyNumY /*= 456211287*/, const int32& InAdditionNumY /*= 762463905*/, const int32& InMultiplyNumZ /*= 606967995*/, const int32& InAdditionNumZ /*= 331255672*/ )
{

	TArray<FVector> ReturnArr;
	int32 SeedX { FMath::FloorToInt32(DSeededRand(InSeed, InMultiplyNumX, InAdditionNumX) * (InSeed+1) * 100)};
	int32 SeedY { FMath::FloorToInt32(DSeededRand(InSeed, InMultiplyNumY, InAdditionNumY) * (InSeed+1) * 100)};
	int32 SeedZ { FMath::FloorToInt32(DSeededRand(InSeed, InMultiplyNumZ, InAdditionNumZ) * (InSeed+1) * 100)};
	ReturnArr.Init(FVector::ZeroVector, Count);
	ParallelFor(Count, [&] (int32 Idx)
	{
		double X = DSeededRandRange(SeedX + Idx, Min, Max, InMultiplyNumX, InAdditionNumX);
		double Y = DSeededRandRange(SeedY + Idx, Min, Max, InMultiplyNumY, InAdditionNumY);
		double Z = 0.0;
		if (!bIgnoreZ)
		{
			Z = DSeededRandRange(SeedZ + Idx, Min, Max, InMultiplyNumY, InAdditionNumY);
		}
		ReturnArr[Idx] = FVector(X, Y, Z);

	}, false);
	return ReturnArr;



}

TArray<double> RandomisationStatics::DSeededRandRangeCountLineSim ( const int32& InSeed, const int32& Count, const double& Min, const double& Max, const int32& InMultiplyNumX /*= 196314165*/, const int32& InAdditionNumX /*= 907633515*/, const int32& InMultiplyNumY /*= 456211287*/, const int32& InAdditionNumY /*= 762463905*/, const int32& InMultiplyNumZ /*= 606967995*/, const int32& InAdditionNumZ /*= 331255672*/ )
{
	TArray<double> ReturnArr;
	ReturnArr.Init(0.0, Count);

	int64 SeedDirection { FMath::FloorToInt64(DSeededRand(InSeed, InMultiplyNumX, InAdditionNumX) * (InSeed+1) * 100)};

	int64 SeedStarting { FMath::FloorToInt64(DSeededRand(InSeed, InMultiplyNumY, InAdditionNumY) * (InSeed+1) * 100)};

	int64 Seed { FMath::FloorToInt64(DSeededRand(InSeed, InMultiplyNumZ, InAdditionNumZ) * (InSeed+1) * 100)};

	FVector DirectionVec { VecSeededRandRange(SeedDirection, -1.0, 1.0, true, InMultiplyNumX, InAdditionNumX, InMultiplyNumY, InAdditionNumY, InMultiplyNumZ, InAdditionNumZ) };
	FVector StartingVec { VecSeededRandRange(SeedStarting, 0, 10.0, true, InMultiplyNumX, InAdditionNumX, InMultiplyNumY, InAdditionNumY, InMultiplyNumZ, InAdditionNumZ) };

	double Size { 10.0 };
	double Area { 100.0 };

	ParallelFor(Count, [&] (int64 Idx)
	{
		double X = DSeededRandRange(Seed + Idx, 0, Area, InMultiplyNumX, InAdditionNumX);
		double LineEquationX { (X - StartingVec.X) / DirectionVec.X };

		double Y { (LineEquationX * DirectionVec.Y) + StartingVec.Y};

		Y = Y - ((FMath::FloorToInt(Y)));

		int64 YEdgeIntersects { FMath::FloorToInt64(Y / Size) };
		Y = Y -	( YEdgeIntersects * Size);

		Y = Min + ( (Max - Min) * Y);

		ReturnArr[Idx] = Y;

	}, false);
	return ReturnArr;

}

TArray<FVector> RandomisationStatics::VecSeededandRangeCountLineSim ( const int32& InSeed, const int32& Count, const bool bBounce, const double& InLineCount, const double& Min, const double& Max, const bool& bIgnoreZ, const double& BoxX, const double& BoxY, const double& BoxZ, const int32& InMultiplyNumX /*= 196314165*/, const int32& InAdditionNumX /*= 907633515*/, const int32& InMultiplyNumY /*= 456211287*/, const int32& InAdditionNumY /*= 762463905*/, const int32& InMultiplyNumZ /*= 606967995*/, const int32& InAdditionNumZ /*= 331255672*/ )
{
	TArray<FVector> ReturnArr;
	if (Count > 0)
	{
		ReturnArr.Init(FVector::ZeroVector, Count);
		int64 SeedDirection { FMath::FloorToInt64(DSeededRand(InSeed, InMultiplyNumX, InAdditionNumX) * (InSeed+1) * 100)};

		int64 SeedStarting { FMath::FloorToInt64(DSeededRand(InSeed, InMultiplyNumY, InAdditionNumY) * (InSeed+1) * 100)};

		int64 Seed { FMath::FloorToInt64(DSeededRand(InSeed, InMultiplyNumZ, InAdditionNumZ) * (InSeed+1) * 100 )};

		TArray<TPair<FVector, FVector>> LinesArray;

		int32 LineCount {FMath::FloorToInt32(InLineCount)};
		for (int32 LineIdx {0}; LineIdx < LineCount ; ++LineIdx)
		{
			FVector DirectionVec { VecSeededRandRange(SeedDirection + LineIdx, -1.0, 1.0, bIgnoreZ, InMultiplyNumX, InAdditionNumX, InMultiplyNumY, InAdditionNumY, InMultiplyNumZ, InAdditionNumZ) };
			FVector StartingVec { VecSeededRandRange(SeedStarting + LineIdx, 0.0, BoxX, 0.0, BoxY, 0.0, BoxZ, bIgnoreZ, InMultiplyNumX, InAdditionNumX, InMultiplyNumY, InAdditionNumY, InMultiplyNumZ, InAdditionNumZ) };
			LinesArray.Add(TPair<FVector, FVector>(DirectionVec, StartingVec));
		}

		double Area { FMath::Abs(bIgnoreZ ? BoxX * BoxY : BoxX * BoxY * BoxZ) };
	
		ParallelFor(Count, [&] (int32 Idx)
		{
			int32 LineToUse { FMath::FloorToInt32(LineCount * DSeededRand(Seed + Idx, InMultiplyNumX, InAdditionNumX) ) };
			if (LineToUse >= LinesArray.Num())
			{
				LineToUse = LinesArray.Num() - 1;
			}

			FVector DirectionVec { LinesArray[LineToUse].Key};
			FVector StartingVec { LinesArray[LineToUse].Value};
			double X = DSeededRandRange(Seed + Idx, 0, Area, InMultiplyNumX, InAdditionNumX);

			double LineEquationX { (X - StartingVec.X) / DirectionVec.X };

			double Y { (LineEquationX * DirectionVec.Y) + StartingVec.Y};
		

			int64 XEdgeIntersects { FMath::FloorToInt64(X / BoxX) };
			int64 YEdgeIntersects { FMath::FloorToInt64(Y / BoxY) };
			X = X - ( XEdgeIntersects * BoxX);
			Y = Y -	( YEdgeIntersects * BoxY);
			if (bBounce)
			{

				if ( (XEdgeIntersects & 0x01 ) == 1) // Odd
				{
					X = BoxX - X;
				}
				if ( (YEdgeIntersects & 0x01 ) == 1) // Odd
				{
					Y = BoxY - Y;
				}
			}
		

			double Z = 0.0;
			if (!bIgnoreZ)
			{
				Z = (LineEquationX * DirectionVec.Z) + StartingVec.Z ;

				int64 ZEdgeIntersects { FMath::FloorToInt64(Z / BoxZ) };
				Z = Z - ( ZEdgeIntersects * BoxZ);
				if (bBounce)
				{
					if ( (ZEdgeIntersects & 0x01 ) == 1) // Odd
					{
						Z = BoxZ - Z;
					}
				}
			}

			ReturnArr[Idx] = FVector(X, Y, Z);

		}, false);
	}
	return ReturnArr;

}

TArray<uint32> RandomisationStatics::ChaosMachineTest ( const uint32& InSeed, const uint32& InCount) 
{


	// Vars
	uint32 Seed { InSeed }; 
	uint32 Increment { 0 }; 
	uint32 Mask {1}; 
	uint32 Temp{0};


	// Constants
	const uint32 Time{ 1 };
	const uint32 Space { 16 };


	// Function to mod
	TFunction<uint32 (uint32 InInt)> ModLam = [](uint32 InInt) { return InInt % Space; };
	
	// Function for bit rotation
	TFunction<uint32 (uint32 InX, uint32 InY)> RotateLam = [](uint32 InX, uint32 InY) { return ((( InX) << (InY )) | (( InX) >> (16 - (InY )))); };

	TArray<uint32> OutArr;
	OutArr.Empty();

	// Variables in the algorithm
	int32 a {0};
	int32 b {0};
	int32 c {0};
	int32 d {0};
	int32 e {0};

	// Initial Secret Key
	TArray<int32> SecretKey {1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9, 3, 2 };
	//SecretKey.Init(0, Space);

	// Push

	Increment = Seed * 0x3C6EF35F;

	for ( Temp = 0; Temp < Space; ++Temp ) // changes values in secretkey using Seed and increment. XOR and bitshifts are used to obfuscate and "randomise" values. Increment is then changed every loop
	{
		// Reinforcement
		uint32 TempValue1 { (( Increment * ( Seed + 1)) ^ Seed ) >> 16 };
		SecretKey[ ModLam(Temp )] ^= TempValue1;

		uint32 TempValue2 { (( Increment * ( Seed - 1)) ^ Seed ) >> 00 };
		SecretKey[ ModLam(Temp )] ^= TempValue2;
	
		// Finalization
		Increment ^= (( SecretKey [ModLam(Increment - 1)] + SecretKey[ModLam (Temp )]) << 16) ^ (( SecretKey[ModLam (Increment + 1)] - SecretKey [ModLam(Temp )]) << 00);
		 
	}


	// Pull

	for (uint32 Idx{0}; Idx < InCount; ++Idx)
	{
		// Variables
		a = SecretKey[ModLam(Increment + 0)]; 
		b = SecretKey[ModLam (Increment + 1)];
		
		// Initialization
		Mask = ( a + Increment) * (b - Seed );
		
		// Chaos
		e = ( SecretKey[ModLam( Mask - b )] << ( a % 9)) ^ ( SecretKey[ModLam( Mask + a )] >> ( b % 9));
		
		// Rounds
		for (Temp = 0; Temp < Time * 2; Temp += 2) 
		{
			// Absorption
			c ^= SecretKey[ModLam (Increment + Temp - 2)]; d ^= SecretKey [ModLam(Increment + Temp + 2)];
			
			// Mixing Modification
			c ^= (d ^= RotateLam(e , c % 17));
			SecretKey[ModLam (Increment + Temp - 2)] -= (d += ( Mask & c ));
			d += (c += RotateLam(Mask , d % 17));
			SecretKey[ModLam (Increment + Temp + 2)] += (c += ( e & d ));
		}


	// Transition
		SecretKey[ModLam (Increment + 0)] = RotateLam (c , Mask % 17) ^ RotateLam(d , Mask % 17) ^ (Mask & a ) ^ ( e & b );
		SecretKey[ModLam (Increment + 1)] = ( b >> 1) ^ ( -( b & 1) & 0xB400 ); // LFSR
		
		// Finalization
		Mask += (c ^ ( b << 8) ^ (d << 16) ^ ( a & 0xFF ) ^ (( a >> 8) << 24));
		
		// Cleaning
		c = 0xFFFF ;
		d = 0xFFFF ;
		
		// Increment
		Increment += 2;
		OutArr.Add(Mask);
	
	}
	
	
	return OutArr;


	
	
	//return OutArr;
}
	
TArray<float> RandomisationStatics::FltChaosMachine ( const uint32& InSeed, const uint32& InCount )
{
	TArray<uint32> ChaosInt {ChaosMachineTest(InSeed, InCount)};
	union { float f; int32 i; } Result;
	union { float f; int32 i; } Temp;
	const float SRandTemp = 1.0f;
	Temp.f = SRandTemp;
	TArray<float> ReturnArr;

	for (uint32 Idx{0}; Idx < InCount; ++Idx)
	{

		Result.i = (Temp.i & 0xff800000) | (ChaosInt[Idx] & 0x007fffff);
		ReturnArr.Add(Result.f - 1.f);	 

	}

	return ReturnArr;
}

