// ============================================================================
//  LABORATORIO 04 - Implementacion del contenedor de plataformas.
// ============================================================================

#include "ContenedorPlataformas.h"

#include "Plataforma.h"

#include "PMLineal.h"
#include "PMVertical.h"
#include "PMCircular.h"
#include "PMOcho.h"
#include "PMEspiral.h"
#include "PMPendulo.h"
#include "PMRotatoria.h"
#include "PMZigzag.h"
#include "PMAleatoria.h"
#include "PMHelicoidal.h"
#include "PMRebote.h"
#include "PMOnda.h"

#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AContenedorPlataformas::AContenedorPlataformas()
{
	PrimaryActorTick.bCanEverTick = true;

	// 240 plataformas: 12 tipos de movimiento x 20 plataformas de cada tipo.
	// El enunciado pide al menos 200.
	CantidadDePlataformas = 240;
	PlataformasPorFila = 16;
	SeparacionEntrePlataformas = 750.0f;
	CentroDelContenedor = FVector(0.0f, 0.0f, 0.0f);

	SegundosDePausa = 5.0f;
	SegundosDeMovimiento = 10.0f;
	IntervaloDeDesaparicion = 0.3f;
	bRepoblarAlAgotarse = false;

	bEnMovimiento = false;
	Ciclo = 0;
	Desaparecidas = 0;
	UltimaDesaparecida = TEXT("-");

	// LOS 12 COMPORTAMIENTOS DE MOVIMIENTO.
	ClasesDeMovimiento.Add(APMLineal::StaticClass());
	ClasesDeMovimiento.Add(APMVertical::StaticClass());
	ClasesDeMovimiento.Add(APMCircular::StaticClass());
	ClasesDeMovimiento.Add(APMOcho::StaticClass());
	ClasesDeMovimiento.Add(APMEspiral::StaticClass());
	ClasesDeMovimiento.Add(APMPendulo::StaticClass());
	ClasesDeMovimiento.Add(APMRotatoria::StaticClass());
	ClasesDeMovimiento.Add(APMZigzag::StaticClass());
	ClasesDeMovimiento.Add(APMAleatoria::StaticClass());
	ClasesDeMovimiento.Add(APMHelicoidal::StaticClass());
	ClasesDeMovimiento.Add(APMRebote::StaticClass());
	ClasesDeMovimiento.Add(APMOnda::StaticClass());
}

void AContenedorPlataformas::BeginPlay()
{
	Super::BeginPlay();

	LlenarContenedor();

	// Recien spawneadas, las plataformas arrancan QUIETAS durante 5 segundos.
	IniciarFasePausa();
}

// ---------------------------------------------------------------------------
//  Llenado del contenedor
// ---------------------------------------------------------------------------

void AContenedorPlataformas::LlenarContenedor()
{
	Plataformas.Empty();
	Plataformas.Reserve(CantidadDePlataformas);

	for (int32 i = 0; i < CantidadDePlataformas; ++i)
	{
		// Los tipos se van alternando, asi que los 12 movimientos quedan
		// repartidos por todo el escenario y no agrupados por zonas.
		const TSubclassOf<APlataforma> Clase = ClasesDeMovimiento[i % ClasesDeMovimiento.Num()];

		if (APlataforma* Nueva = CrearPlataforma(Clase, i, CalcularAncla(i)))
		{
			Plataformas.Add(Nueva);
		}
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Cyan,
			FString::Printf(TEXT("CONTENEDOR lleno: %d plataformas, %d tipos de movimiento"),
				Plataformas.Num(), ClasesDeMovimiento.Num()));
	}

	UE_LOG(LogTemp, Warning, TEXT("[Contenedor] %d plataformas creadas (%d tipos de movimiento)"),
		Plataformas.Num(), ClasesDeMovimiento.Num());
}

FVector AContenedorPlataformas::CalcularAncla(int32 IndiceDePlataforma) const
{
	const int32 Columna = IndiceDePlataforma % PlataformasPorFila;
	const int32 Fila = IndiceDePlataforma / PlataformasPorFila;
	const int32 TotalDeFilas = FMath::CeilToInt(static_cast<float>(CantidadDePlataformas) / PlataformasPorFila);

	// Rejilla centrada en CentroDelContenedor.
	const float X = (Columna - (PlataformasPorFila - 1) * 0.5f) * SeparacionEntrePlataformas;
	const float Y = (Fila - (TotalDeFilas - 1) * 0.5f) * SeparacionEntrePlataformas;

	// Cinco alturas distintas: el conjunto se ve como un escenario en capas.
	const float Z = 260.0f + (IndiceDePlataforma % 5) * 150.0f;

	return CentroDelContenedor + FVector(X, Y, Z);
}

APlataforma* AContenedorPlataformas::CrearPlataforma(TSubclassOf<APlataforma> ClasePlataforma,
	int32 IndiceDePlataforma, const FVector& Ancla)
{
	UWorld* Mundo = GetWorld();
	if (!Mundo || !ClasePlataforma)
	{
		return nullptr;
	}

	// Spawn diferido: hay que darle su ancla ANTES del BeginPlay, porque sobre
	// ese punto se calcula todo su recorrido.
	const FTransform Transformacion(FRotator::ZeroRotator, Ancla);

	APlataforma* Nueva = Mundo->SpawnActorDeferred<APlataforma>(ClasePlataforma, Transformacion);
	if (!Nueva)
	{
		return nullptr;
	}

	Nueva->Configurar(IndiceDePlataforma, Ancla);
	UGameplayStatics::FinishSpawningActor(Nueva, Transformacion);

	return Nueva;
}

// ---------------------------------------------------------------------------
//  Ciclo:  5 s quietas  ->  10 s moviendose  ->  5 s quietas  ->  ...
// ---------------------------------------------------------------------------

void AContenedorPlataformas::IniciarFasePausa()
{
	bEnMovimiento = false;

	LimpiarDestruidas();

	for (APlataforma* Plataforma : Plataformas)
	{
		Plataforma->IniciarPausa();
	}

	// Mientras estan quietas no desaparece ninguna.
	GetWorldTimerManager().ClearTimer(TemporizadorDeDesaparicion);

	if (Plataformas.Num() == 0)
	{
		if (!bRepoblarAlAgotarse)
		{
			UE_LOG(LogTemp, Warning, TEXT("[Contenedor] no quedan plataformas: el ciclo termina"));

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red,
					FString::Printf(TEXT("CONTENEDOR VACIO: desaparecieron las %d plataformas"), Desaparecidas));
			}
			return;
		}

		// Opcional: volver a llenarlo para que la demostracion no se corte.
		LlenarContenedor();
	}

	UE_LOG(LogTemp, Warning, TEXT("[Contenedor] PAUSA de %.1f s  (quedan %d plataformas)"),
		SegundosDePausa, Plataformas.Num());

	GetWorldTimerManager().SetTimer(TemporizadorDeFase, this,
		&AContenedorPlataformas::IniciarFaseMovimiento, SegundosDePausa, false);
}

void AContenedorPlataformas::IniciarFaseMovimiento()
{
	bEnMovimiento = true;
	++Ciclo;

	LimpiarDestruidas();

	// Cada plataforma retoma SU movimiento donde lo habia dejado.
	for (APlataforma* Plataforma : Plataformas)
	{
		Plataforma->IniciarMovimiento();
	}

	UE_LOG(LogTemp, Warning, TEXT("[Contenedor] MOVIMIENTO %d de %.1f s  (quedan %d plataformas)"),
		Ciclo, SegundosDeMovimiento, Plataformas.Num());

	// Durante el movimiento, cada 0.3 s desaparece una plataforma al azar.
	GetWorldTimerManager().SetTimer(TemporizadorDeDesaparicion, this,
		&AContenedorPlataformas::DesaparecerPlataformaAlAzar, IntervaloDeDesaparicion, true);

	// Y a los 10 s se vuelve a la pausa.
	GetWorldTimerManager().SetTimer(TemporizadorDeFase, this,
		&AContenedorPlataformas::IniciarFasePausa, SegundosDeMovimiento, false);
}

// ---------------------------------------------------------------------------
//  Desaparicion aleatoria cada 0.3 segundos
// ---------------------------------------------------------------------------

void AContenedorPlataformas::DesaparecerPlataformaAlAzar()
{
	LimpiarDestruidas();

	if (Plataformas.Num() == 0)
	{
		GetWorldTimerManager().ClearTimer(TemporizadorDeDesaparicion);
		return;
	}

	// AL AZAR: se sortea una posicion cualquiera del contenedor.
	const int32 Sorteada = FMath::RandRange(0, Plataformas.Num() - 1);

	APlataforma* Elegida = Plataformas[Sorteada];
	Plataformas.RemoveAt(Sorteada);

	UltimaDesaparecida = FString::Printf(TEXT("%s #%d"), *Elegida->ObtenerTipo(), Elegida->ObtenerIndice());
	++Desaparecidas;

	Elegida->Desaparecer();

	UE_LOG(LogTemp, Verbose, TEXT("[Contenedor] desaparece %s  (quedan %d)"),
		*UltimaDesaparecida, Plataformas.Num());
}

void AContenedorPlataformas::LimpiarDestruidas()
{
	Plataformas.RemoveAll([](APlataforma* Plataforma) { return !IsValid(Plataforma); });
}

// ---------------------------------------------------------------------------
//  Informacion en pantalla
// ---------------------------------------------------------------------------

void AContenedorPlataformas::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MostrarEstadoEnPantalla();
}

void AContenedorPlataformas::MostrarEstadoEnPantalla()
{
	if (!GEngine)
	{
		return;
	}

	const float Restante = GetWorldTimerManager().GetTimerRemaining(TemporizadorDeFase);

	// Claves fijas: los mensajes se reemplazan en lugar de apilarse.
	GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::White,
		FString::Printf(TEXT("Ultima en desaparecer: %s"), *UltimaDesaparecida));

	GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::White,
		FString::Printf(TEXT("Desaparecidas: %d      En escena: %d"), Desaparecidas, Plataformas.Num()));

	GEngine->AddOnScreenDebugMessage(3, 1.0f, bEnMovimiento ? FColor::Green : FColor::Yellow,
		bEnMovimiento
			? FString::Printf(TEXT("MOVIMIENTO %d  -  vuelven a la pausa en %.1f s"), Ciclo, FMath::Max(Restante, 0.0f))
			: FString::Printf(TEXT("PAUSA  -  se mueven en %.1f s"), FMath::Max(Restante, 0.0f)));
}
