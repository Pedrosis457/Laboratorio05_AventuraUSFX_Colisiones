// LABORATORIO 04 - Movimiento 6: pendulo colgado de un punto alto.

#include "PMPendulo.h"

APMPendulo::APMPendulo()
{
	Tipo = TEXT("PMPendulo");
	Frecuencia = 1.3f;
	LongitudDeCuerda = 520.0f;
	AnguloMaximo = 55.0f;

	ConfigurarApariencia(FVector(2.30f, 1.60f, 0.35f), FLinearColor(0.95f, 0.45f, 0.10f));
}

void APMPendulo::SortearParametros()
{
	Super::SortearParametros();

	LongitudDeCuerda *= FMath::FRandRange(0.75f, 1.25f);
	AnguloMaximo *= FMath::FRandRange(0.70f, 1.20f);
}

void APMPendulo::Mover(float /*DeltaTime*/)
{
	// El pivote esta encima del ancla; la plataforma cuelga de el.
	const FVector Pivote = Ancla + FVector(0.0f, 0.0f, LongitudDeCuerda);

	const float Angulo = FMath::DegreesToRadians(AnguloMaximo)
		* FMath::Sin(TiempoEnMovimiento * Frecuencia + Desfase);

	// Brazo del pendulo: baja desde el pivote y se abre sobre el eje sorteado.
	const FVector Brazo =
		Eje * (LongitudDeCuerda * FMath::Sin(Angulo)) -
		FVector::UpVector * (LongitudDeCuerda * FMath::Cos(Angulo));

	SetActorLocation(Pivote + Brazo);

	// La plataforma se inclina igual que la cuerda.
	SetActorRotation(FRotator(0.0f, Eje.Rotation().Yaw, FMath::RadiansToDegrees(Angulo)));
}
