// LABORATORIO 04 - Movimiento 3: orbita circular horizontal.

#include "PMCircular.h"

APMCircular::APMCircular()
{
	Tipo = TEXT("PMCircular");
	Radio = 300.0f;
	Frecuencia = 1.2f;

	ConfigurarApariencia(FVector(2.00f, 2.00f, 0.35f), FLinearColor(0.90f, 0.25f, 0.20f));
}

void APMCircular::Mover(float /*DeltaTime*/)
{
	// Sentido vale +1 o -1: la mitad de las plataformas gira al reves.
	const float Angulo = TiempoEnMovimiento * Frecuencia * Sentido + Desfase;

	const FVector Desplazamiento(
		FMath::Cos(Angulo) * Radio,
		FMath::Sin(Angulo) * Radio,
		0.0f);

	SetActorLocation(Ancla + Desplazamiento);

	// Se orienta hacia donde avanza, como un carrusel.
	SetActorRotation(FRotator(0.0f, FMath::RadiansToDegrees(Angulo) + 90.0f * Sentido, 0.0f));
}
