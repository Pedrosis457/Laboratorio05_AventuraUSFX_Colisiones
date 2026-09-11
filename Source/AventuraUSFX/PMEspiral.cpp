// LABORATORIO 04 - Movimiento 5: espiral que se abre y se cierra.

#include "PMEspiral.h"

APMEspiral::APMEspiral()
{
	Tipo = TEXT("PMEspiral");
	Radio = 380.0f;
	Frecuencia = 2.2f;
	FrecuenciaRadial = 0.6f;

	ConfigurarApariencia(FVector(1.80f, 1.80f, 0.35f), FLinearColor(0.10f, 0.80f, 0.55f));
}

void APMEspiral::SortearParametros()
{
	Super::SortearParametros();

	FrecuenciaRadial *= FMath::FRandRange(0.75f, 1.25f);
}

void APMEspiral::Mover(float /*DeltaTime*/)
{
	const float Angulo = TiempoEnMovimiento * Frecuencia * Sentido + Desfase;

	// El radio va de un 25% a un 100%: la espiral se abre y se vuelve a cerrar.
	const float RadioActual = Radio * (0.25f + 0.75f * FMath::Abs(FMath::Sin(TiempoEnMovimiento * FrecuenciaRadial)));

	const FVector Desplazamiento(
		FMath::Cos(Angulo) * RadioActual,
		FMath::Sin(Angulo) * RadioActual,
		RadioActual * 0.45f);   // sube al abrirse, baja al cerrarse

	SetActorLocation(Ancla + Desplazamiento);
}
