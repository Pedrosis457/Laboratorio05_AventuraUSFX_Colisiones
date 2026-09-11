// LABORATORIO 04 - Movimiento 12: cabalga una ola.

#include "PMOnda.h"

APMOnda::APMOnda()
{
	Tipo = TEXT("PMOnda");
	Amplitud = 340.0f;
	Frecuencia = 0.9f;
	AmplitudVertical = 170.0f;
	LongitudDeOnda = 260.0f;
	VelocidadDeOnda = 2.4f;

	ConfigurarApariencia(FVector(2.60f, 1.80f, 0.35f), FLinearColor(0.10f, 0.45f, 0.35f));
}

void APMOnda::Mover(float /*DeltaTime*/)
{
	// Recorrido horizontal de ida y vuelta.
	const float Recorrido = FMath::Sin(TiempoEnMovimiento * Frecuencia + Desfase) * Amplitud;

	// Fase de la ola en el punto donde esta la plataforma: la ola la atraviesa.
	const float FaseDeOnda = Recorrido / LongitudDeOnda + TiempoEnMovimiento * VelocidadDeOnda + Desfase;

	const float Altura = AmplitudVertical * (1.0f + FMath::Sin(FaseDeOnda));

	SetActorLocation(Ancla + Eje * Recorrido + FVector(0.0f, 0.0f, Altura));

	// Se inclina segun la pendiente de la ola (la derivada del seno es el coseno).
	const float Inclinacion = FMath::RadiansToDegrees(FMath::Atan(FMath::Cos(FaseDeOnda) * 0.6f));

	SetActorRotation(FRotator(Inclinacion, Eje.Rotation().Yaw, 0.0f));
}
