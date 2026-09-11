// ============================================================================
//  LABORATORIO 05 - Implementacion del GameMode.
// ============================================================================

#include "AventuraUSFXGameModeBase.h"

#include "ContenedorPlataformas.h"
#include "BolaDemoledora.h"
#include "ZonaDeEnergia.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"

AAventuraUSFXGameModeBase::AAventuraUSFXGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CentroDelEscenario = FVector::ZeroVector;
	Contenedor = nullptr;

	CantidadDeBolas = 18;
	CantidadDeZonas = 2;
}

void AAventuraUSFXGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	PrepararEscenario();

	// El contenedor se crea por codigo; en su BeginPlay llena el TArray con las
	// 240 plataformas y arranca el ciclo pausa / movimiento.
	if (UWorld* Mundo = GetWorld())
	{
		FActorSpawnParameters Parametros;
		Parametros.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		Contenedor = Mundo->SpawnActor<AContenedorPlataformas>(
			AContenedorPlataformas::StaticClass(), CentroDelEscenario, FRotator::ZeroRotator, Parametros);
	}

	// LAS DOS APLICACIONES DE COLISIONES DEL LABORATORIO 05.
	ColocarZonasDeEnergia();     // OVERLAP (begin - end)
	SoltarBolasDemoledoras();    // BLOCK

	GetWorldTimerManager().SetTimer(TemporizadorJugador, this,
		&AAventuraUSFXGameModeBase::ColocarJugador, 0.2f, false);

	UE_LOG(LogTemp, Warning, TEXT("[GameMode] escenario listo: %d bolas (BLOCK) y %d zonas (OVERLAP)"),
		Bolas.Num(), Zonas.Num());
}

void AAventuraUSFXGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MostrarMarcadorDeColisiones();
}

// ---------------------------------------------------------------------------
//  APLICACION 1:  BLOCK
// ---------------------------------------------------------------------------

void AAventuraUSFXGameModeBase::SoltarBolasDemoledoras()
{
	UWorld* Mundo = GetWorld();
	if (!Mundo)
	{
		return;
	}

	FActorSpawnParameters Parametros;
	Parametros.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int32 i = 0; i < CantidadDeBolas; ++i)
	{
		// Se sueltan repartidas por encima de la rejilla y a distintas alturas,
		// para que no lleguen todas al mismo tiempo.
		const FVector Punto = CentroDelEscenario + FVector(
			FMath::FRandRange(-5200.0f, 5200.0f),
			FMath::FRandRange(-5200.0f, 5200.0f),
			FMath::FRandRange(3000.0f, 9000.0f));

		if (ABolaDemoledora* Bola = Mundo->SpawnActor<ABolaDemoledora>(
			ABolaDemoledora::StaticClass(), Punto, FRotator::ZeroRotator, Parametros))
		{
			Bolas.Add(Bola);
		}
	}
}

// ---------------------------------------------------------------------------
//  APLICACION 2:  OVERLAP (begin - end)
// ---------------------------------------------------------------------------

void AAventuraUSFXGameModeBase::ColocarZonasDeEnergia()
{
	UWorld* Mundo = GetWorld();
	if (!Mundo)
	{
		return;
	}

	// Dos zonas que barren en cruz: una de norte a sur y otra de este a oeste.
	const FVector Direcciones[2] = { FVector(0.0f, 1.0f, 0.0f), FVector(1.0f, 0.0f, 0.0f) };
	const FLinearColor Colores[2] = { FLinearColor(0.10f, 1.00f, 0.35f), FLinearColor(1.00f, 0.20f, 0.90f) };

	for (int32 i = 0; i < CantidadDeZonas; ++i)
	{
		const FVector Centro = CentroDelEscenario + FVector(0.0f, 0.0f, 500.0f);
		const FTransform Transformacion(FRotator::ZeroRotator, Centro);

		AZonaDeEnergia* Zona = Mundo->SpawnActorDeferred<AZonaDeEnergia>(
			AZonaDeEnergia::StaticClass(), Transformacion);

		if (!Zona)
		{
			continue;
		}

		// Spawn diferido: la barrida se configura antes de que corra su BeginPlay.
		Zona->ConfigurarBarrida(Centro, Direcciones[i % 2], 5000.0f, Colores[i % 2]);
		UGameplayStatics::FinishSpawningActor(Zona, Transformacion);

		Zonas.Add(Zona);
	}
}

// ---------------------------------------------------------------------------
//  Marcador en pantalla
// ---------------------------------------------------------------------------

void AAventuraUSFXGameModeBase::MostrarMarcadorDeColisiones()
{
	if (!GEngine)
	{
		return;
	}

	int32 Golpes = 0;
	int32 Rotas = 0;
	for (const ABolaDemoledora* Bola : Bolas)
	{
		if (IsValid(Bola))
		{
			Golpes += Bola->ObtenerGolpes();
			Rotas += Bola->ObtenerRoturas();
		}
	}

	int32 Dentro = 0;
	int32 Entradas = 0;
	int32 Salidas = 0;
	for (const AZonaDeEnergia* Zona : Zonas)
	{
		if (IsValid(Zona))
		{
			Dentro += Zona->ObtenerPlataformasDentro();
			Entradas += Zona->ObtenerEntradas();
			Salidas += Zona->ObtenerSalidas();
		}
	}

	// Claves fijas: los mensajes se reemplazan en lugar de apilarse.
	GEngine->AddOnScreenDebugMessage(21, 1.0f, FColor::Green,
		FString::Printf(TEXT("OVERLAP  begin: %d   end: %d   dentro ahora: %d"), Entradas, Salidas, Dentro));

	GEngine->AddOnScreenDebugMessage(22, 1.0f, FColor::Orange,
		FString::Printf(TEXT("BLOCK    golpes de bola: %d   plataformas rotas: %d"), Golpes, Rotas));

	GEngine->AddOnScreenDebugMessage(23, 1.0f, FColor::White,
		TEXT("--- COLISIONES (Laboratorio 05) ---"));
}

// ---------------------------------------------------------------------------
//  Escenario
// ---------------------------------------------------------------------------

void AAventuraUSFXGameModeBase::PrepararEscenario()
{
	// Suelo general, solido, por debajo de todas las plataformas.
	CrearPlano(CentroDelEscenario, FVector(220.0f, 220.0f, 1.0f),
		FLinearColor(0.10f, 0.12f, 0.16f), true);
}

void AAventuraUSFXGameModeBase::CrearPlano(const FVector& Posicion, const FVector& Escala,
	const FLinearColor& Color, bool bSolido)
{
	UWorld* Mundo = GetWorld();
	if (!Mundo)
	{
		return;
	}

	FActorSpawnParameters Parametros;
	Parametros.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* Plano = Mundo->SpawnActor<AActor>(AActor::StaticClass(), Posicion, FRotator::ZeroRotator, Parametros);
	if (!Plano)
	{
		return;
	}

	UStaticMeshComponent* Componente = NewObject<UStaticMeshComponent>(Plano);
	Componente->SetMobility(EComponentMobility::Movable);

	if (bSolido)
	{
		// El suelo tambien BLOQUEA: es contra el que rebotan las bolas.
		Componente->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Componente->SetCollisionProfileName(TEXT("BlockAll"));
	}
	else
	{
		Componente->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (UStaticMesh* MallaPlano = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane.Plane")))
	{
		Componente->SetStaticMesh(MallaPlano);
	}

	if (UMaterial* MaterialBasico = LoadObject<UMaterial>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial")))
	{
		Componente->SetMaterial(0, MaterialBasico);
	}

	Componente->SetRelativeScale3D(Escala);
	Plano->SetRootComponent(Componente);
	Componente->RegisterComponent();

	if (UMaterialInstanceDynamic* Dinamico = Componente->CreateAndSetMaterialInstanceDynamic(0))
	{
		Dinamico->SetVectorParameterValue(TEXT("Color"), Color);
	}
}

void AAventuraUSFXGameModeBase::ColocarJugador()
{
	APlayerController* Controlador = UGameplayStatics::GetPlayerController(this, 0);
	if (!Controlador)
	{
		return;
	}

	// Punto de partida: atras y en alto, con toda la rejilla de plataformas a la vista.
	const FVector PosicionInicial = CentroDelEscenario + FVector(0.0f, -11000.0f, 5200.0f);
	const FRotator RotacionInicial(-22.0f, 90.0f, 0.0f);

	if (APawn* Jugador = Controlador->GetPawn())
	{
		Jugador->SetActorLocationAndRotation(PosicionInicial, RotacionInicial);
	}

	Controlador->SetControlRotation(RotacionInicial);
	Controlador->bShowMouseCursor = false;
	Controlador->SetInputMode(FInputModeGameOnly());

	MostrarControles();
}

void AAventuraUSFXGameModeBase::MostrarControles()
{
	if (!GEngine)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(10, 600.0f, FColor::White, TEXT("ESC  salir"));
	GEngine->AddOnScreenDebugMessage(11, 600.0f, FColor::White, TEXT("Mouse  mirar alrededor"));
	GEngine->AddOnScreenDebugMessage(12, 600.0f, FColor::White, TEXT("Q / Ctrl  bajar"));
	GEngine->AddOnScreenDebugMessage(13, 600.0f, FColor::White, TEXT("E / Espacio  subir"));
	GEngine->AddOnScreenDebugMessage(14, 600.0f, FColor::White, TEXT("W A S D  desplazarse"));
	GEngine->AddOnScreenDebugMessage(15, 600.0f, FColor::Yellow, TEXT("--- CONTROLES ---"));
}
