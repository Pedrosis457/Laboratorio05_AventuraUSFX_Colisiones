// LABORATORIO 04 - Movimiento 1: vaiven en linea recta horizontal.

#include "PMLineal.h"

APMLineal::APMLineal()
{
	Tipo = TEXT("PMLineal");
	Amplitud = 320.0f;
	Frecuencia = 1.4f;

	ConfigurarApariencia(FVector(2.40f, 2.40f, 0.35f), FLinearColor(0.15f, 0.65f, 0.95f));
}

void APMLineal::Mover(float /*DeltaTime*/)
{
	// Un seno sobre el eje sorteado: recorrido de ida y vuelta suave.
	const float Recorrido = FMath::Sin(TiempoEnMovimiento * Frecuencia + Desfase) * Amplitud;

	SetActorLocation(Ancla + Eje * Recorrido);
}
