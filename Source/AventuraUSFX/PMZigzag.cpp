// LABORATORIO 04 - Movimiento 8: zigzag de esquinas marcadas.

#include "PMZigzag.h"

APMZigzag::APMZigzag()
{
	Tipo = TEXT("PMZigzag");
	Amplitud = 300.0f;
	Frecuencia = 0.8f;
	FrecuenciaTransversal = 3.6f;

	ConfigurarApariencia(FVector(1.90f, 1.90f, 0.35f), FLinearColor(0.20f, 0.35f, 0.75f));
}

void APMZigzag::Mover(float /*DeltaTime*/)
{
	// Dos ondas TRIANGULARES (no senos): por eso los giros son en punta.
	const float Avance = OndaTriangular(TiempoEnMovimiento * Frecuencia + Desfase) * Amplitud;
	const float Cruce = OndaTriangular(TiempoEnMovimiento * FrecuenciaTransversal + Desfase) * (Amplitud * 0.45f);

	SetActorLocation(Ancla + Eje * Avance + EjePerpendicular() * Cruce);
}
