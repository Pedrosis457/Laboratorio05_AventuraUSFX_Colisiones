// ============================================================================
//  LABORATORIO 05 - Implementacion de la clase padre Plataforma.
// ============================================================================

#include "Plataforma.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

APlataforma::APlataforma()
{
	PrimaryActorTick.bCanEverTick = true;

	// --- Raiz vacia ----------------------------------------------------------
	Raiz = CreateDefaultSubobject<USceneComponent>(TEXT("Raiz"));
	RootComponent = Raiz;

	// --- Cuerpo visible ------------------------------------------------------
	Malla = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Malla"));
	Malla->SetupAttachment(Raiz);
	Malla->SetMobility(EComponentMobility::Movable);

	// ================================================================
	//  COLISIONES (Laboratorio 05)
	//
	//  BlockAllDynamic: objeto WorldDynamic que BLOQUEA todo lo demas.
	//  Es lo que hace que las bolas demoledoras reboten contra la
	//  plataforma en lugar de atravesarla, y lo que permite pararse
	//  encima de ella.
	//
	//  Ademas se activan los eventos de solapamiento: aunque la
	//  plataforma responda Block, frente a una zona que responde
	//  Overlap manda la respuesta mas debil de las dos, asi que el par
	//  plataforma/zona genera BeginOverlap y EndOverlap.
	// ================================================================
	Malla->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Malla->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	Malla->SetGenerateOverlapEvents(true);

	// Con cientos de plataformas en escena no conviene que todas hagan sombra.
	Malla->SetCastShadow(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MallaCubo(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (MallaCubo.Succeeded())
	{
		Malla->SetStaticMesh(MallaCubo.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialBasico(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (MaterialBasico.Succeeded())
	{
		Malla->SetMaterial(0, MaterialBasico.Object);
	}

	// Plataforma ancha y baja: 220 x 220 x 35 cm.
	Malla->SetRelativeScale3D(FVector(2.20f, 2.20f, 0.35f));

	// --- Valores por defecto -------------------------------------------------
	Tipo = TEXT("Plataforma");
	Indice = 0;
	Estado = EEstadoPlataforma::Pausada;

	Ancla = FVector::ZeroVector;
	TiempoEnMovimiento = 0.0f;

	Velocidad = 300.0f;
	Amplitud = 250.0f;
	Frecuencia = 1.0f;
	Radio = 250.0f;
	Desfase = 0.0f;
	Sentido = 1.0f;
	Eje = FVector(1.0f, 0.0f, 0.0f);

	Resistencia = 3;
	GolpesRecibidos = 0;
	bEnergizada = false;
	MultiplicadorDeEnergia = 1.0f;
	ColorDeEnergia = FLinearColor::White;

	Color = FLinearColor(0.70f, 0.70f, 0.70f);
	MaterialDinamico = nullptr;
}

void APlataforma::ConfigurarApariencia(const FVector& Escala, const FLinearColor& NuevoColor)
{
	Malla->SetRelativeScale3D(Escala);
	Color = NuevoColor;
}

void APlataforma::Configurar(int32 NuevoIndice, const FVector& NuevaAncla)
{
	Indice = NuevoIndice;
	Ancla = NuevaAncla;

	SortearParametros();
}

void APlataforma::SortearParametros()
{
	Desfase = FMath::FRandRange(0.0f, 2.0f * PI);
	Sentido = FMath::RandBool() ? 1.0f : -1.0f;

	const float Angulo = FMath::FRandRange(0.0f, 2.0f * PI);
	Eje = FVector(FMath::Cos(Angulo), FMath::Sin(Angulo), 0.0f);

	// Variacion del +-25%: dos plataformas de la misma clase nunca van iguales.
	Amplitud *= FMath::FRandRange(0.75f, 1.25f);
	Frecuencia *= FMath::FRandRange(0.75f, 1.25f);
	Radio *= FMath::FRandRange(0.75f, 1.25f);
	Velocidad *= FMath::FRandRange(0.75f, 1.25f);
}

void APlataforma::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(Ancla);

	if (Malla)
	{
		// Se guarda la inclinacion que dejo el constructor de la subclase.
		RotacionBaseDeMalla = Malla->GetRelativeRotation();

		MaterialDinamico = Malla->CreateAndSetMaterialInstanceDynamic(0);
		ActualizarColor();
	}

	// Nace quieta: el contenedor la mantiene asi los primeros 5 segundos.
	Estado = EEstadoPlataforma::Pausada;
}

void APlataforma::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// En pausa no se toca nada: el reloj no avanza y Mover() no se llama.
	if (Estado != EEstadoPlataforma::EnMovimiento)
	{
		return;
	}

	// Dentro de una zona de energia el multiplicador es mayor que 1, asi que
	// la plataforma recorre su misma trayectoria mas rapido. Ese es el efecto
	// visible de la colision por OVERLAP.
	const float DeltaEfectivo = DeltaTime * MultiplicadorDeEnergia;

	TiempoEnMovimiento += DeltaEfectivo;

	// Llamada polimorfica: aqui entra el movimiento particular de cada subclase.
	Mover(DeltaEfectivo);
}

void APlataforma::Mover(float /*DeltaTime*/)
{
	// La plataforma base no se desplaza. Cada subclase redefine este metodo.
}

void APlataforma::IniciarPausa()
{
	Estado = EEstadoPlataforma::Pausada;
}

void APlataforma::IniciarMovimiento()
{
	Estado = EEstadoPlataforma::EnMovimiento;
}

void APlataforma::Desaparecer()
{
	UE_LOG(LogTemp, Verbose, TEXT("[Plataforma %d] %s desaparece"), Indice, *Tipo);

	Destroy();
}

// ---------------------------------------------------------------------------
//  LABORATORIO 05 - Respuesta a las colisiones
// ---------------------------------------------------------------------------

bool APlataforma::RecibirGolpe(const FVector& /*PuntoDeImpacto*/)
{
	++GolpesRecibidos;

	UE_LOG(LogTemp, Verbose, TEXT("[BLOCK] %s #%d recibe el golpe %d de %d"),
		*Tipo, Indice, GolpesRecibidos, Resistencia);

	if (GolpesRecibidos >= Resistencia)
	{
		// Aguanto los golpes que podia: se rompe.
		Desaparecer();
		return true;
	}

	// Todavia aguanta: queda mas cerca del rojo.
	ActualizarColor();
	return false;
}

void APlataforma::EntrarEnZonaDeEnergia(const FLinearColor& ColorDeLaZona, float Multiplicador)
{
	bEnergizada = true;
	ColorDeEnergia = ColorDeLaZona;
	MultiplicadorDeEnergia = Multiplicador;

	ActualizarColor();
}

void APlataforma::SalirDeZonaDeEnergia()
{
	bEnergizada = false;
	MultiplicadorDeEnergia = 1.0f;

	// Vuelve a su color propio, con el dano de los golpes que ya tenia.
	ActualizarColor();
}

void APlataforma::ActualizarColor()
{
	if (!MaterialDinamico)
	{
		return;
	}

	FLinearColor Objetivo = Color;

	// Los golpes de las bolas la van dejando al rojo vivo.
	if (GolpesRecibidos > 0 && Resistencia > 0)
	{
		const float Dano = FMath::Clamp(static_cast<float>(GolpesRecibidos) / Resistencia, 0.0f, 1.0f);
		Objetivo = FMath::Lerp(Color, FLinearColor(1.0f, 0.03f, 0.0f), Dano);
	}

	// Mientras esta dentro de la zona manda el color de la zona.
	if (bEnergizada)
	{
		Objetivo = ColorDeEnergia;
	}

	MaterialDinamico->SetVectorParameterValue(TEXT("Color"), Objetivo);
}

float APlataforma::OndaTriangular(float Fase)
{
	// Fase en radianes -> vuelta normalizada 0..1 -> triangulo entre -1 y 1.
	const float Vuelta = FMath::Frac(Fase / (2.0f * PI) + 1.0f);
	return 4.0f * FMath::Abs(Vuelta - 0.5f) - 1.0f;
}
