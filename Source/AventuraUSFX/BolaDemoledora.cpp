// ============================================================================
//  LABORATORIO 05 - APLICACION 1 DE COLISIONES: BLOCK.
// ============================================================================

#include "BolaDemoledora.h"

#include "Plataforma.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

ABolaDemoledora::ABolaDemoledora()
{
	PrimaryActorTick.bCanEverTick = true;

	Esfera = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Esfera"));
	RootComponent = Esfera;

	Esfera->SetMobility(EComponentMobility::Movable);

	// ================================================================
	//  CONFIGURACION DE LA COLISION  ->  BLOCK
	//
	//  PhysicsActor: objeto de tipo PhysicsBody que BLOQUEA a los demas.
	//  Con la fisica activada, el bloqueo se resuelve como un choque real:
	//  la bola rebota contra la plataforma en lugar de atravesarla.
	//
	//  SetNotifyRigidBodyCollision(true) es imprescindible: sin el, el
	//  choque ocurre pero el motor NO llama a OnComponentHit.
	// ================================================================
	Esfera->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Esfera->SetCollisionProfileName(TEXT("PhysicsActor"));
	Esfera->SetSimulatePhysics(true);
	Esfera->SetNotifyRigidBodyCollision(true);
	Esfera->SetCastShadow(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MallaEsfera(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (MallaEsfera.Succeeded())
	{
		Esfera->SetStaticMesh(MallaEsfera.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialBasico(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (MaterialBasico.Succeeded())
	{
		Esfera->SetMaterial(0, MaterialBasico.Object);
	}

	Esfera->SetRelativeScale3D(FVector(1.40f, 1.40f, 1.40f));

	AlturaDeSalida = 6500.0f;
	AlturaDeRescate = -800.0f;
	EmpujeInicial = 520.0f;
	RadioDeCaida = 5200.0f;

	GolpesDados = 0;
	PlataformasRotas = 0;
	MaterialDinamico = nullptr;
}

void ABolaDemoledora::BeginPlay()
{
	Super::BeginPlay();

	// Se engancha el evento de la colision BLOCK.
	Esfera->OnComponentHit.AddDynamic(this, &ABolaDemoledora::AlGolpear);

	// Mas pesada de lo normal para que las plataformas no la frenen.
	Esfera->SetMassOverrideInKg(NAME_None, 400.0f, true);

	MaterialDinamico = Esfera->CreateAndSetMaterialInstanceDynamic(0);
	if (MaterialDinamico)
	{
		MaterialDinamico->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.02f, 0.02f, 0.04f));
	}

	Lanzar(GetActorLocation());
}

void ABolaDemoledora::Lanzar(const FVector& PuntoDeSalida)
{
	SetActorLocation(PuntoDeSalida);

	Esfera->SetPhysicsLinearVelocity(FVector::ZeroVector);
	Esfera->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

	// Empujon horizontal sorteado: cada bola cae por un lado distinto.
	const float Angulo = FMath::FRandRange(0.0f, 2.0f * PI);

	Esfera->SetPhysicsLinearVelocity(FVector(
		FMath::Cos(Angulo) * EmpujeInicial,
		FMath::Sin(Angulo) * EmpujeInicial,
		0.0f));
}

void ABolaDemoledora::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Si rodo hasta salirse del escenario, se la vuelve a soltar desde arriba:
	// asi la demostracion de la colision BLOCK no se corta nunca.
	if (GetActorLocation().Z < AlturaDeRescate)
	{
		Lanzar(FVector(
			FMath::FRandRange(-RadioDeCaida, RadioDeCaida),
			FMath::FRandRange(-RadioDeCaida, RadioDeCaida),
			AlturaDeSalida));
	}
}

// ---------------------------------------------------------------------------
//  EVENTO DE LA COLISION BLOCK
// ---------------------------------------------------------------------------

void ABolaDemoledora::AlGolpear(UPrimitiveComponent* /*ComponenteGolpeador*/, AActor* OtroActor,
	UPrimitiveComponent* /*OtroComponente*/, FVector /*ImpulsoNormal*/, const FHitResult& Golpe)
{
	// Solo interesan los golpes contra plataformas: contra el suelo no.
	APlataforma* Plataforma = Cast<APlataforma>(OtroActor);
	if (!Plataforma)
	{
		return;
	}

	++GolpesDados;

	// Se guarda el nombre ANTES del golpe: si se rompe, el actor se destruye.
	const FString TipoGolpeado = Plataforma->ObtenerTipo();

	// La plataforma decide si aguanta o se rompe.
	if (Plataforma->RecibirGolpe(Golpe.ImpactPoint))
	{
		++PlataformasRotas;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Orange,
				FString::Printf(TEXT("BLOCK: bola rompe %s"), *TipoGolpeado));
		}
	}
}
