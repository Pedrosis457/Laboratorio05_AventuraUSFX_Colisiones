// ============================================================================
//  LABORATORIO 04 - Programacion Avanzada (SIS457)
//
//  EL CONTENEDOR DE PLATAFORMAS.
//
//  Guarda 240 plataformas (el enunciado pide al menos 200) en un TArray y es
//  el que marca el ritmo de todas ellas con sus objetos FTimerHandle:
//
//      TemporizadorDeFase         5 s quietas  ->  10 s en movimiento  ->  ...
//      TemporizadorDeDesaparicion cada 0.3 s retira una plataforma al azar,
//                                 y solo corre durante la fase de movimiento.
//
//  Las 240 plataformas se reparten entre las 12 clases de movimiento, asi que
//  hay 20 plataformas de cada tipo y cada una lleva sus propios parametros
//  sorteados: se mueven de manera independiente unas de otras.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ContenedorPlataformas.generated.h"

class APlataforma;

UCLASS()
class AVENTURAUSFX_API AContenedorPlataformas : public AActor
{
	GENERATED_BODY()

public:
	AContenedorPlataformas();

	virtual void Tick(float DeltaTime) override;

	/** Cuantas plataformas siguen en el escenario. */
	int32 ContarPlataformas() const { return Plataformas.Num(); }

protected:
	virtual void BeginPlay() override;

	// ---------------------------------------------------------------- Llenado

	/** Crea las 240 plataformas y las guarda en el contenedor. */
	void LlenarContenedor();

	APlataforma* CrearPlataforma(TSubclassOf<APlataforma> ClasePlataforma, int32 IndiceDePlataforma,
		const FVector& Ancla);

	/** Posicion de rejilla que le toca a la plataforma numero IndiceDePlataforma. */
	FVector CalcularAncla(int32 IndiceDePlataforma) const;

	// ---------------------------------------------------------------- Ciclo

	/** Fase quieta: ordena la pausa a todas y apaga la desaparicion. */
	void IniciarFasePausa();

	/** Fase de movimiento: suelta a todas y enciende la desaparicion cada 0.3 s. */
	void IniciarFaseMovimiento();

	/** Se ejecuta cada 0.3 s: elige una plataforma al azar y la retira. */
	void DesaparecerPlataformaAlAzar();

	/** Saca del TArray las plataformas que ya no existen. */
	void LimpiarDestruidas();

	void MostrarEstadoEnPantalla();

	// ---------------------------------------------------------------- Contenido

	/** EL CONTENEDOR: aqui viven las plataformas mientras estan en escena. */
	UPROPERTY(VisibleAnywhere, Category = "Contenedor")
	TArray<APlataforma*> Plataformas;

	/** Las 12 clases de movimiento que se reparten entre las plataformas. */
	UPROPERTY()
	TArray<TSubclassOf<APlataforma>> ClasesDeMovimiento;

	// ---------------------------------------------------------------- Ajustes

	UPROPERTY(EditAnywhere, Category = "Contenedor")
	int32 CantidadDePlataformas;

	UPROPERTY(EditAnywhere, Category = "Contenedor")
	int32 PlataformasPorFila;

	UPROPERTY(EditAnywhere, Category = "Contenedor")
	float SeparacionEntrePlataformas;

	UPROPERTY(EditAnywhere, Category = "Contenedor")
	FVector CentroDelContenedor;

	UPROPERTY(EditAnywhere, Category = "Contenedor|Ciclo")
	float SegundosDePausa;

	UPROPERTY(EditAnywhere, Category = "Contenedor|Ciclo")
	float SegundosDeMovimiento;

	UPROPERTY(EditAnywhere, Category = "Contenedor|Ciclo")
	float IntervaloDeDesaparicion;

	/**
	 * Si esta activo, cuando no queda ninguna plataforma el contenedor se
	 * vuelve a llenar y el ciclo empieza de nuevo. Apagado por defecto: el
	 * enunciado no lo pide, pero es comodo para grabar el video.
	 */
	UPROPERTY(EditAnywhere, Category = "Contenedor|Ciclo")
	bool bRepoblarAlAgotarse;

	// ---------------------------------------------------------------- Estado

	/** OBJETO FTimerHandle del ciclo pausa / movimiento. */
	FTimerHandle TemporizadorDeFase;

	/** OBJETO FTimerHandle de la desaparicion aleatoria cada 0.3 segundos. */
	FTimerHandle TemporizadorDeDesaparicion;

	UPROPERTY(VisibleAnywhere, Category = "Contenedor|Estado")
	bool bEnMovimiento;

	UPROPERTY(VisibleAnywhere, Category = "Contenedor|Estado")
	int32 Ciclo;

	UPROPERTY(VisibleAnywhere, Category = "Contenedor|Estado")
	int32 Desaparecidas;

	FString UltimaDesaparecida;
};
