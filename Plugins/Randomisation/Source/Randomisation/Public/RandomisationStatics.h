// Luke Jackson - 09/08/24
// Place to test different randomisation methods 

#pragma once

#include "CoreMinimal.h"

class RANDOMISATION_API RandomisationStatics
{
public:
	/*
	*	Uses same method as FMath::SRand , Adding option to change Multiply and addition numbers so the same seed could get different numbers generated
	*
	*/
	static float FSeededRand(const int32& InSeed, const int32& InMultiplyNum = 196314165, const int32& InAdditionNum = 907633515);
	static double DSeededRand(const int32& InSeed, const int32& InMultiplyNum = 196314165, const int32& InAdditionNum = 907633515);

	static int32 I32Rand();

	static float FSeededRandRange(const int32& InSeed, const float& Min, const float& Max, const int32& InMultiplyNum = 196314165, const int32& InAdditionNum = 907633515);
	static double DSeededRandRange(const int32& InSeed, const double& Min, const double& Max, const int32& InMultiplyNum = 196314165, const int32& InAdditionNum = 907633515);

	static FVector VecSeededRand(const int32& InSeed, const bool& IgnoreZ, const int32& InMultiplyNum = 196314165, const int32& InAdditionNum = 907633515);
	static FVector VecSeededRandRange(const int32& InSeed, const double& Min, const double& Max, const bool& bIgnoreZ, const int32& InMultiplyNumX = 196314165, const int32& InAdditionNumX = 907633515, const int32& InMultiplyNumY = 456211287, const int32& InAdditionNumY = 762463905, const int32& InMultiplyNumZ = 606967995, const int32& InAdditionNumZ = 331255672);
	static FVector VecSeededRandRange(const int32& InSeed, const double& MinX, const double& MaxX, const double& MinY, const double& MaxY, const double& MinZ, const double& MaxZ, const bool& bIgnoreZ, const int32& InMultiplyNumX = 196314165, const int32& InAdditionNumX = 907633515, const int32& InMultiplyNumY = 456211287, const int32& InAdditionNumY = 762463905, const int32& InMultiplyNumZ = 606967995, const int32& InAdditionNumZ = 331255672);

	static TArray<double> DSeededRandRangeCount(const int32& InSeed, const int32& Count, const double& Min, const double& Max, const int32& InMultiplyNum = 196314165, const int32& InAdditionNum = 907633515);

	static TArray<FVector> VecSeededRandRangeCount(const int32& InSeed, const int32& Count, const double& Min, const double& Max, const bool& bIgnoreZ, const int32& InMultiplyNumX = 196314165, const int32& InAdditionNumX = 907633515, const int32& InMultiplyNumY = 456211287, const int32& InAdditionNumY = 762463905, const int32& InMultiplyNumZ = 606967995, const int32& InAdditionNumZ = 331255672);

	// Currently not working
	static TArray<double> DSeededRandRangeCountLineSim(const int32& InSeed, const int32& Count, const double& Min, const double& Max, const int32& InMultiplyNumX = 196314165, const int32& InAdditionNumX = 907633515, const int32& InMultiplyNumY = 456211287, const int32& InAdditionNumY = 762463905, const int32& InMultiplyNumZ = 606967995, const int32& InAdditionNumZ = 331255672);

	static TArray<FVector> VecSeededandRangeCountLineSim(const int32& InSeed, const int32& Count, const bool bBounce, const double& InLineCount, const double& Min, const double& Max, const bool& bIgnoreZ, const double& BoxX, const double&  BoxY, const double& BoxZ, const int32& InMultiplyNumX = 196314165, const int32& InAdditionNumX = 907633515, const int32& InMultiplyNumY = 456211287, const int32& InAdditionNumY = 762463905, const int32& InMultiplyNumZ = 606967995, const int32& InAdditionNumZ = 331255672);


	// Copied and adapted method from (Maciej A. Czyzewski, 2016, Chaos Machine: Different Approach to the Application and Significance of Numbers)
	static TArray<uint32> ChaosMachineTest(const uint32& InSeed, const uint32& InCount);

	static TArray<float> FltChaosMachine(const uint32& InSeed, const uint32& InCount );
};