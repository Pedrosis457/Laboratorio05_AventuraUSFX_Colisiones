// ============================================================================
//  LABORATORIO 05 - Programacion Avanzada (SIS457)
//
//  El GameMode arma el escenario y coloca:
//      - el contenedor de plataformas             (viene del Laboratorio 04)
//      - las bolas demoledoras   -> colision BLOCK
//      - las zonas de energia    -> colision OVERLAP (begin - end)
//
//  Todo se genera por codigo C++: no hay nada que configurar en el editor.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AventuraUSFXGameModeBase.generated.h"

class AContenedorPlataformas;
class ABolaDemoledora;
class AZonaDeEnergia;

UCLASS()
class AVENTURAUSFX_API AAventuraUSFXGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAventuraUSFXGameModeBase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

protected:
	void PrepararEscenario();

	void CrearPlano(const FVector& Posicion, const FVector& Escala, const FLinearColor& Color, bool bSolido);

	/** APLICACION BLOCK: suelta las bolas demoledoras sobre el escenario. */
	void SoltarBolasDemoledoras();

	/** APLICACION OVERLAP: coloca las zonas de energia que barren el escenario. */
	void ColocarZonasDeEnergia();

	void ColocarJugador();

	void MostrarControles();

	void MostrarMarcadorDeColisiones();

	/** El unico contenedor de la escena: el que guarda las 240 plataformas. */
	UPROPERTY()
	AContenedorPlataformas* Contenedor;

	UPROPERTY()
	TArray<ABolaDemoledora*> Bolas;

	UPROPERTY()
	TArray<AZonaDeEnergia*> Zonas;

	UPROPERTY(EditAnywhere, Category = "Escenario")
	FVector CentroDelEscenario;

	UPROPERTY(EditAnywhere, Category = "Colisiones|Block")
	int32 CantidadDeBolas;

	UPROPERTY(EditAnywhere, Category = "Colisiones|Overlap")
	int32 CantidadDeZonas;

	FTimerHandle TemporizadorJugador;
};
