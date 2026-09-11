// ============================================================================
//  LABORATORIO 05 - APLICACION 2 DE COLISIONES: OVERLAP (begin - end).
// ============================================================================

#include "ZonaDeEnergia.h"

#include "Plataforma.h"

#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"

AZonaDeEnergia::AZonaDeEnergia()
{
	PrimaryActorTick.bCanEverTick = true;

	RadioDeLaZona = 1100.0f;
	MultiplicadorDeEnergia = 2.5f;

	Esfera = CreateDefaultSubobject<USphereComponent>(TEXT("Esfera"));
	RootComponent = Esfera;

	Esfera->SetMobility(EComponentMobility::Movable);
	Esfera->InitSphereRadius(RadioDeLaZona);

	// ================================================================
	//  CONFIGURACION DE LA COLISION  ->  OVERLAP
	//
	//  QueryOnly + perfil "Trigger": la zona no participa de la fisica,
	//  no empuja ni detiene a nadie. Su respuesta a todos los canales es
	//  Overlap, y lo unico que hace es AVISAR cuando algo entra o sale.
	//
	//  La plataforma responde Block, pero entre dos respuestas distintas
	//  manda siempre la mas debil: Overlap. Por eso el par zona/plataforma
	//  genera BeginOverlap y EndOverlap en vez de bloquearse.
	// ================================================================
	Esfera->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Esfera->SetCollisionProfileName(TEXT("Trigger"));
	Esfera->SetGenerateOverlapEvents(true);

	CentroDeBarrida = FVector::ZeroVector;
	DireccionDeBarrida = FVector(0.0f, 1.0f, 0.0f);
	AlcanceDeBarrida = 5000.0f;
	VelocidadDeBarrida = 0.45f;
	TiempoDeVida = 0.0f;

	PlataformasDentro = 0;
	Entradas = 0;
	Salidas = 0;
	ColorDeLaZona = FLinearColor(0.10f, 1.00f, 0.35f);
}

void AZonaDeEnergia::ConfigurarBarrida(const FVector& NuevoCentro, const FVector& NuevaDireccion,
	float NuevoAlcance, const FLinearColor& NuevoColor)
{
	CentroDeBarrida = NuevoCentro;
	DireccionDeBarrida = NuevaDireccion.GetSafeNormal();
	AlcanceDeBarrida = NuevoAlcance;
	ColorDeLaZona = NuevoColor;
}

void AZonaDeEnergia::BeginPlay()
{
	Super::BeginPlay();

	Esfera->SetSphereRadius(RadioDeLaZona);

	// Se enganchan los DOS eventos del overlap: el de entrada y el de salida.
	Esfera->OnComponentBeginOverlap.AddDynamic(this, &AZonaDeEnergia::AlEntrar);
	Esfera->OnComponentEndOverlap.AddDynamic(this, &AZonaDeEnergia::AlSalir);

	UE_LOG(LogTemp, Warning, TEXT("[OVERLAP] zona de energia lista, radio %.0f"), RadioDeLaZona);
}

void AZonaDeEnergia::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TiempoDeVida += DeltaTime;

	// La zona BARRE el escenario de ida y vuelta. Al moverse, el motor
	// recalcula los solapamientos: de ahi salen los begin y los end.
	const float Recorrido = FMath::Sin(TiempoDeVida * VelocidadDeBarrida) * AlcanceDeBarrida;

	SetActorLocation(CentroDeBarrida + DireccionDeBarrida * Recorrido);

	// Se dibuja el contorno para que en el video se vea por donde va la zona.
	DrawDebugSphere(GetWorld(), GetActorLocation(), RadioDeLaZona, 20,
		ColorDeLaZona.ToFColor(true), false, -1.0f, 0, 8.0f);
}

// ---------------------------------------------------------------------------
//  EVENTOS DEL OVERLAP
// ---------------------------------------------------------------------------

void AZonaDeEnergia::AlEntrar(UPrimitiveComponent* /*ComponenteDeLaZona*/, AActor* OtroActor,
	UPrimitiveComponent* /*OtroComponente*/, int32 /*OtroIndice*/, bool /*bDeBarrido*/,
	const FHitResult& /*Barrido*/)
{
	// A la zona solo le interesan las plataformas.
	APlataforma* Plataforma = Cast<APlataforma>(OtroActor);
	if (!Plataforma)
	{
		return;
	}

	++PlataformasDentro;
	++Entradas;

	// BEGIN: se enciende.
	Plataforma->EntrarEnZonaDeEnergia(ColorDeLaZona, MultiplicadorDeEnergia);

	UE_LOG(LogTemp, Verbose, TEXT("[OVERLAP begin] entra %s #%d  (dentro: %d)"),
		*Plataforma->ObtenerTipo(), Plataforma->ObtenerIndice(), PlataformasDentro);
}

void AZonaDeEnergia::AlSalir(UPrimitiveComponent* /*ComponenteDeLaZona*/, AActor* OtroActor,
	UPrimitiveComponent* /*OtroComponente*/, int32 /*OtroIndice*/)
{
	APlataforma* Plataforma = Cast<APlataforma>(OtroActor);
	if (!Plataforma)
	{
		return;
	}

	PlataformasDentro = FMath::Max(0, PlataformasDentro - 1);
	++Salidas;

	// END: se apaga y vuelve a ser la de antes.
	Plataforma->SalirDeZonaDeEnergia();

	UE_LOG(LogTemp, Verbose, TEXT("[OVERLAP end] sale %s #%d  (dentro: %d)"),
		*Plataforma->ObtenerTipo(), Plataforma->ObtenerIndice(), PlataformasDentro);
}
