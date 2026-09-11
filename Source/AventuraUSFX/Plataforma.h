// ============================================================================
//  LABORATORIO 05 - Programacion Avanzada (SIS457)
//  Universidad San Francisco Xavier de Chuquisaca
//
//  CLASE PADRE de todas las plataformas (viene del Laboratorio 04).
//
//      APlataforma
//        |-- APMLineal      |-- APMPendulo     |-- APMHelicoidal
//        |-- APMVertical    |-- APMRotatoria   |-- APMRebote
//        |-- APMCircular    |-- APMZigzag      |-- APMOnda
//        |-- APMOcho        |-- APMAleatoria
//        |-- APMEspiral
//
//  NUEVO EN EL LABORATORIO 05: la plataforma sabe responder a las dos formas
//  de colision que pide el enunciado.
//
//      BLOCK    RecibirGolpe()           <- la golpea una ABolaDemoledora
//      OVERLAP  EntrarEnZonaDeEnergia()  <- begin: entra a una AZonaDeEnergia
//               SalirDeZonaDeEnergia()   <- end:   sale de la zona
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Plataforma.generated.h"

class UStaticMeshComponent;
class UMaterialInstanceDynamic;

/** Los dos estados del ciclo pausa / movimiento. */
UENUM()
enum class EEstadoPlataforma : uint8
{
	Pausada,
	EnMovimiento
};

UCLASS()
class AVENTURAUSFX_API APlataforma : public AActor
{
	GENERATED_BODY()

public:
	APlataforma();

	virtual void Tick(float DeltaTime) override;

	/**
	 * Alta de la plataforma dentro del contenedor. Se llama ANTES del BeginPlay
	 * (spawn diferido) porque fija el punto de anclaje sobre el que se calcula
	 * todo el movimiento.
	 */
	void Configurar(int32 NuevoIndice, const FVector& NuevaAncla);

	/** Ordenes que da el contenedor al cambiar de fase. */
	void IniciarPausa();
	void IniciarMovimiento();

	/** La retira del escenario (la dispara el temporizador de 0.3 s). */
	void Desaparecer();

	// ---------------------------------------------- LABORATORIO 05: colisiones

	/**
	 * COLISION BLOCK. La golpeo una bola demoledora: el choque es fisico (las
	 * dos mallas se bloquean) y ademas avisa por evento Hit.
	 * @return true si con este golpe la plataforma se rompio.
	 */
	bool RecibirGolpe(const FVector& PuntoDeImpacto);

	/** COLISION OVERLAP - BEGIN: la cubrio una zona de energia. */
	void EntrarEnZonaDeEnergia(const FLinearColor& ColorDeLaZona, float Multiplicador);

	/** COLISION OVERLAP - END: la plataforma dejo atras la zona de energia. */
	void SalirDeZonaDeEnergia();

	bool EstaEnergizada() const { return bEnergizada; }

	FString ObtenerTipo() const { return Tipo; }
	int32 ObtenerIndice() const { return Indice; }
	EEstadoPlataforma ObtenerEstado() const { return Estado; }

protected:
	virtual void BeginPlay() override;

	/**
	 * COMPORTAMIENTO DE MOVIMIENTO PARTICULAR.
	 * El padre no se desplaza: cada subclase redefine este metodo y ahi esta
	 * la diferencia entre un tipo de plataforma y otro.
	 */
	virtual void Mover(float DeltaTime);

	/**
	 * Sortea los parametros propios de esta plataforma. Se llama desde
	 * Configurar(), de modo que dos plataformas de la misma clase nunca se
	 * mueven al unisono: cada una lleva su desfase, su sentido y su tamano.
	 */
	virtual void SortearParametros();

	/** Utilitario que usan las subclases dentro de su constructor. */
	void ConfigurarApariencia(const FVector& Escala, const FLinearColor& NuevoColor);

	/** Repinta la malla segun el dano recibido y si esta o no energizada. */
	void ActualizarColor();

	// ---------------------------------------------------------------- Componentes

	/**
	 * La raiz es un componente vacio, NO la malla. Varias subclases rotan la
	 * malla en local (giro sobre si misma, cabeceo); si la malla fuera la raiz
	 * ese giro sobrescribiria la posicion de la plataforma en el mundo.
	 */
	UPROPERTY(VisibleAnywhere, Category = "Plataforma|Componentes")
	USceneComponent* Raiz;

	UPROPERTY(VisibleAnywhere, Category = "Plataforma|Componentes")
	UStaticMeshComponent* Malla;

	// ---------------------------------------------------------------- Identidad

	UPROPERTY(EditAnywhere, Category = "Plataforma")
	FString Tipo;

	UPROPERTY(VisibleAnywhere, Category = "Plataforma")
	int32 Indice;

	UPROPERTY(VisibleAnywhere, Category = "Plataforma")
	EEstadoPlataforma Estado;

	// ---------------------------------------------------------------- Movimiento

	/** Punto fijo del escenario sobre el que se calcula todo el recorrido. */
	UPROPERTY(VisibleAnywhere, Category = "Plataforma|Movimiento")
	FVector Ancla;

	/** Reloj propio: solo corre mientras el estado es EnMovimiento. */
	UPROPERTY(VisibleAnywhere, Category = "Plataforma|Movimiento")
	float TiempoEnMovimiento;

	UPROPERTY(EditAnywhere, Category = "Plataforma|Movimiento")
	float Velocidad;

	UPROPERTY(EditAnywhere, Category = "Plataforma|Movimiento")
	float Amplitud;

	UPROPERTY(EditAnywhere, Category = "Plataforma|Movimiento")
	float Frecuencia;

	UPROPERTY(EditAnywhere, Category = "Plataforma|Movimiento")
	float Radio;

	/** Desfase inicial sorteado, en radianes. */
	UPROPERTY(VisibleAnywhere, Category = "Plataforma|Movimiento")
	float Desfase;

	/** +1 o -1: sentido en que recorre su trayectoria. */
	UPROPERTY(VisibleAnywhere, Category = "Plataforma|Movimiento")
	float Sentido;

	/** Direccion horizontal sorteada sobre la que trabajan varios movimientos. */
	UPROPERTY(VisibleAnywhere, Category = "Plataforma|Movimiento")
	FVector Eje;

	// ---------------------------------------------- LABORATORIO 05: colisiones

	/** Golpes de bola que aguanta antes de romperse (colision BLOCK). */
	UPROPERTY(EditAnywhere, Category = "Plataforma|Colisiones")
	int32 Resistencia;

	UPROPERTY(VisibleAnywhere, Category = "Plataforma|Colisiones")
	int32 GolpesRecibidos;

	/** Verdadero mientras esta dentro de una zona de energia (colision OVERLAP). */
	UPROPERTY(VisibleAnywhere, Category = "Plataforma|Colisiones")
	bool bEnergizada;

	/** Cuanto mas rapido se mueve mientras esta energizada. */
	UPROPERTY(VisibleAnywhere, Category = "Plataforma|Colisiones")
	float MultiplicadorDeEnergia;

	/** Color que le presta la zona mientras esta dentro. */
	FLinearColor ColorDeEnergia;

	// ---------------------------------------------------------------- Apariencia

	FLinearColor Color;

	UPROPERTY()
	UMaterialInstanceDynamic* MaterialDinamico;

	// ---------------------------------------------------------------- Auxiliares

	/** Rotacion con la que la subclase dejo la malla en su constructor. */
	FRotator RotacionBaseDeMalla;

	/** Perpendicular horizontal de Eje. Util para trayectorias en el plano. */
	FVector EjePerpendicular() const { return FVector(-Eje.Y, Eje.X, 0.0f); }

	/** Onda triangular de periodo 2*PI y salida entre -1 y 1 (usada en zigzag). */
	static float OndaTriangular(float Fase);
};
