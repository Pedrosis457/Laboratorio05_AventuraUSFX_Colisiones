// LABORATORIO 04 - Movimiento 10: helice, sube girando como un tornillo.

#include "PMHelicoidal.h"
#include "Components/StaticMeshComponent.h"

APMHelicoidal::APMHelicoidal()
{
	Tipo = TEXT("PMHelicoidal");
	Radio = 260.0f;
	Frecuencia = 2.0f;
	AlturaDeHelice = 620.0f;
	FrecuenciaVertical = 0.7f;

	ConfigurarApariencia(FVector(2.00f, 2.00f, 0.35f), FLinearColor(0.20f, 0.90f, 0.90f));
}

void APMHelicoidal::SortearParametros()
{
	Super::SortearParametros();

	AlturaDeHelice *= FMath::FRandRange(0.75f, 1.25f);
	FrecuenciaVertical *= FMath::FRandRange(0.75f, 1.25f);
}

void APMHelicoidal::Mover(float DeltaTime)
{
	const float Angulo = TiempoEnMovimiento * Frecuencia * Sentido + Desfase;

	// Circulo en el plano + ascenso y descenso: eso es la helice.
	const float Altura = (0.5f + 0.5f * FMath::Sin(TiempoEnMovimiento * FrecuenciaVertical + Desfase)) * AlturaDeHelice;

	const FVector Desplazamiento(
		FMath::Cos(Angulo) * Radio,
		FMath::Sin(Angulo) * Radio,
		Altura);

	SetActorLocation(Ancla + Desplazamiento);

	// La malla ademas gira sobre si misma, como el paso de un tornillo.
	if (Malla)
	{
		Malla->AddLocalRotation(FRotator(0.0f, 90.0f * Sentido * DeltaTime, 0.0f));
	}
}
