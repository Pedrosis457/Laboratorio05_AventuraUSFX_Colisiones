// LABORATORIO 04 - Movimiento 4: recorrido en forma de ocho (lemniscata).

#include "PMOcho.h"

APMOcho::APMOcho()
{
	Tipo = TEXT("PMOcho");
	Radio = 340.0f;
	Frecuencia = 1.0f;

	ConfigurarApariencia(FVector(2.10f, 2.10f, 0.35f), FLinearColor(0.55f, 0.20f, 0.85f));
}

void APMOcho::Mover(float /*DeltaTime*/)
{
	// Lemniscata de Gerono:  x = R*sen(a)   y = R*sen(a)*cos(a)
	const float Angulo = TiempoEnMovimiento * Frecuencia * Sentido + Desfase;

	const float X = Radio * FMath::Sin(Angulo);
	const float Y = Radio * FMath::Sin(Angulo) * FMath::Cos(Angulo);

	// El ocho se dibuja sobre el eje sorteado, no siempre sobre el eje X global.
	SetActorLocation(Ancla + Eje * X + EjePerpendicular() * Y);
}
