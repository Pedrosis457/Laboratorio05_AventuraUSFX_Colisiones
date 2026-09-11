// LABORATORIO 04 - Movimiento 9: caminata a destinos sorteados.

#include "PMAleatoria.h"

APMAleatoria::APMAleatoria()
{
	Tipo = TEXT("PMAleatoria");
	Velocidad = 320.0f;
	Radio = 360.0f;
	DistanciaDeLlegada = 40.0f;

	ConfigurarApariencia(FVector(2.00f, 2.00f, 0.35f), FLinearColor(0.95f, 0.30f, 0.60f));
}

void APMAleatoria::SortearParametros()
{
	Super::SortearParametros();

	ElegirNuevoDestino();
}

void APMAleatoria::ElegirNuevoDestino()
{
	const float Angulo = FMath::FRandRange(0.0f, 2.0f * PI);
	const float RadioSorteado = FMath::FRandRange(Radio * 0.35f, Radio);

	Destino = Ancla + FVector(
		FMath::Cos(Angulo) * RadioSorteado,
		FMath::Sin(Angulo) * RadioSorteado,
		FMath::FRandRange(0.0f, 220.0f));
}

void APMAleatoria::Mover(float DeltaTime)
{
	const FVector Posicion = GetActorLocation();
	FVector Direccion = Destino - Posicion;
	const float Distancia = Direccion.Size();

	if (Distancia <= DistanciaDeLlegada)
	{
		ElegirNuevoDestino();
		return;
	}

	Direccion /= Distancia;

	// Avance a velocidad constante hacia el destino, sin pasarse de largo.
	SetActorLocation(Posicion + Direccion * FMath::Min(Velocidad * DeltaTime, Distancia));
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), Direccion.Rotation(), DeltaTime, 4.0f));
}
