# AventuraUSFX — Laboratorio 05 · Colisiones **block** y **overlap**

**Materia:** Programación Avanzada (SIS457)
**Motor:** Unreal Engine 4.27 — C++
**Base:** código de `AventuraUSFX` del Laboratorio 04 (contenedor de 240 plataformas
con 12 tipos de movimiento)

---

## 1. Qué pedía el laboratorio

> A partir del código de AventuraUSFX, implemente **dos aplicaciones de colisiones**,
> una correspondiente a **block** y otra a **overlap (begin - end)**; la aplicación debe
> corresponder con su creatividad. Se debe defender el código desarrollado.

## 2. Las dos aplicaciones, en una línea

| | Clase nueva | Respuesta de colisión | Evento que usa | Qué se ve |
|---|---|---|---|---|
| **BLOCK** | `ABolaDemoledora` | `Block` (física real) | `OnComponentHit` | Bolas de demolición caen, **rebotan** contra las plataformas y al tercer golpe las rompen |
| **OVERLAP** | `AZonaDeEnergia` | `Overlap` | `OnComponentBeginOverlap` / `OnComponentEndOverlap` | Dos zonas **barren** el escenario; las plataformas que las atraviesan se encienden y al salir se apagan |

La diferencia que demuestra el laboratorio: **la bola no puede atravesar la plataforma**
(block), **la zona sí** (overlap). Y por eso cada una usa un evento distinto — `Hit` solo
existe cuando hay bloqueo; `BeginOverlap`/`EndOverlap` solo existen cuando se deja pasar.

---

## 3. Aplicación BLOCK — `ABolaDemoledora`

Una esfera con física real que se suelta desde 6 000 cm de altura sobre la rejilla de
plataformas. Se configura así:

```cpp
//  PhysicsActor: objeto de tipo PhysicsBody que BLOQUEA a los demas.
//  Con la fisica activada, el bloqueo se resuelve como un choque real:
//  la bola rebota contra la plataforma en lugar de atravesarla.
//
//  SetNotifyRigidBodyCollision(true) es imprescindible: sin el, el
//  choque ocurre pero el motor NO llama a OnComponentHit.
Esfera->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
Esfera->SetCollisionProfileName(TEXT("PhysicsActor"));
Esfera->SetSimulatePhysics(true);
Esfera->SetNotifyRigidBodyCollision(true);
```

Del otro lado, la plataforma responde también `Block` (perfil `BlockAllDynamic`). **Las
dos se bloquean mutuamente**, así que el motor resuelve el choque físicamente y además
avisa con el evento `Hit`, que se engancha en el `BeginPlay`:

```cpp
// Se engancha el evento de la colision BLOCK.
Esfera->OnComponentHit.AddDynamic(this, &ABolaDemoledora::AlGolpear);
```

Y el manejador descuenta resistencia a la plataforma golpeada:

```cpp
void ABolaDemoledora::AlGolpear(UPrimitiveComponent* ComponenteGolpeador, AActor* OtroActor,
    UPrimitiveComponent* OtroComponente, FVector ImpulsoNormal, const FHitResult& Golpe)
{
    // Solo interesan los golpes contra plataformas: contra el suelo no.
    APlataforma* Plataforma = Cast<APlataforma>(OtroActor);
    if (!Plataforma) { return; }

    ++GolpesDados;

    // La plataforma decide si aguanta o se rompe.
    if (Plataforma->RecibirGolpe(Golpe.ImpactPoint)) { ++PlataformasRotas; }
}
```

La plataforma aguanta **3 golpes**: en cada uno se acerca más al rojo, y al tercero se
destruye.

```cpp
bool APlataforma::RecibirGolpe(const FVector& PuntoDeImpacto)
{
    ++GolpesRecibidos;

    if (GolpesRecibidos >= Resistencia)
    {
        Desaparecer();   // aguanto los golpes que podia: se rompe
        return true;
    }

    ActualizarColor();   // todavia aguanta: queda mas cerca del rojo
    return false;
}
```

**Detalle defendible:** en `AlGolpear` el nombre de la plataforma se copia **antes** de
llamar a `RecibirGolpe()`, porque si el golpe la rompe el actor queda destruido y ya no se
le puede pedir nada. Y las bolas que ruedan fuera del escenario se vuelven a soltar desde
arriba (`Tick` → `Lanzar()`), así la demostración no se corta nunca.

---

## 4. Aplicación OVERLAP (begin - end) — `AZonaDeEnergia`

Una esfera de colisión de 1 100 cm de radio que **no detiene a nadie**, solo avisa:

```cpp
//  QueryOnly + perfil "Trigger": la zona no participa de la fisica,
//  no empuja ni detiene a nadie. Su respuesta a todos los canales es
//  Overlap, y lo unico que hace es AVISAR cuando algo entra o sale.
Esfera->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
Esfera->SetCollisionProfileName(TEXT("Trigger"));
Esfera->SetGenerateOverlapEvents(true);
```

Se enganchan **los dos eventos**, el de entrada y el de salida:

```cpp
Esfera->OnComponentBeginOverlap.AddDynamic(this, &AZonaDeEnergia::AlEntrar);
Esfera->OnComponentEndOverlap.AddDynamic(this, &AZonaDeEnergia::AlSalir);
```

| Evento | Qué hace la plataforma |
|---|---|
| `AlEntrar` (**begin**) | `EntrarEnZonaDeEnergia()` → toma el color de la zona y su `MultiplicadorDeEnergia` pasa a **2.5** |
| `AlSalir` (**end**) | `SalirDeZonaDeEnergia()` → vuelve a su color y el multiplicador regresa a **1.0** |

El multiplicador es lo que hace visible el overlap, porque escala el tiempo con el que la
plataforma recorre su trayectoria:

```cpp
// Dentro de una zona de energia el multiplicador es mayor que 1, asi que
// la plataforma recorre su misma trayectoria mas rapido.
const float DeltaEfectivo = DeltaTime * MultiplicadorDeEnergia;

TiempoEnMovimiento += DeltaEfectivo;
Mover(DeltaEfectivo);
```

**El punto fino que hay que saber defender:** la plataforma responde `Block` y la zona
responde `Overlap`. ¿Qué gana? **Siempre la respuesta más débil de las dos** (Ignore <
Overlap < Block). Por eso el par zona/plataforma **se solapa** en lugar de bloquearse, y
por eso la plataforma necesita `SetGenerateOverlapEvents(true)` aunque su perfil sea de
bloqueo:

```cpp
Malla->SetCollisionProfileName(TEXT("BlockAllDynamic"));
Malla->SetGenerateOverlapEvents(true);
```

**Por qué la zona se mueve:** si estuviera quieta, las plataformas que nacen dentro nunca
saldrían y el evento `end` casi no se vería. Por eso **barre** el escenario de un extremo a
otro, y a su paso va encendiendo plataformas y dejándolas apagadas detrás:

```cpp
// La zona BARRE el escenario de ida y vuelta. Al moverse, el motor
// recalcula los solapamientos: de ahi salen los begin y los end.
const float Recorrido = FMath::Sin(TiempoDeVida * VelocidadDeBarrida) * AlcanceDeBarrida;

SetActorLocation(CentroDeBarrida + DireccionDeBarrida * Recorrido);
```

Son **dos zonas cruzadas** (una barre de norte a sur en verde, otra de este a oeste en
rosado) y su contorno se dibuja con `DrawDebugSphere` para que se vea en el video por dónde
van pasando.

---

## 5. Cómo se comprueba que funcionan

El GameMode lleva un marcador en pantalla que se actualiza en cada `Tick`:

```
--- COLISIONES (Laboratorio 05) ---
BLOCK    golpes de bola: 137   plataformas rotas: 24
OVERLAP  begin: 412   end: 389   dentro ahora: 23
```

- Que **`begin` sea mayor que `end`** es correcto: la diferencia son las plataformas que
  están dentro de una zona *en ese instante*.
- Si una plataforma se destruye estando dentro de la zona, el motor dispara igual su
  `EndOverlap`, así que el contador no se desbalancea.

## 6. Cómo ejecutarlo

1. Abrir `AventuraUSFX.uproject` con **Unreal Engine 4.27**
2. Presionar **Play**

Controles: **W A S D** desplazarse · **Mouse** mirar · **E/Espacio** subir ·
**Q/Ctrl** bajar · **Esc** salir.

Todo se genera por código desde el `GameMode` (suelo, contenedor, bolas, zonas y posición
del jugador): **no hay que configurar nada en el editor**.

## 7. Estructura del código

```
Source/AventuraUSFX/
├── BolaDemoledora.h / .cpp             ← APLICACION BLOCK (OnComponentHit)
├── ZonaDeEnergia.h / .cpp              ← APLICACION OVERLAP (Begin/EndOverlap)
│
├── Plataforma.h / .cpp                 clase padre + RecibirGolpe() y Entrar/SalirDeZona()
├── ContenedorPlataformas.h / .cpp      contenedor de 240 plataformas (Laboratorio 04)
├── PMLineal … PMOnda                   los 12 tipos de movimiento (Laboratorio 04)
└── AventuraUSFXGameModeBase.h / .cpp   arma el escenario y suelta bolas y zonas
```

## 8. Lo que viene del Laboratorio 04

El escenario es el mismo: un `AContenedorPlataformas` con **240 plataformas** repartidas
entre **12 clases de movimiento**, que alternan **5 s quietas / 10 s moviéndose** con un
`FTimerHandle`, y de las que **desaparece una al azar cada 0.3 s** durante el movimiento.
Sobre eso se montaron las dos aplicaciones de colisiones de este laboratorio.

## 9. Video explicativo

*(pendiente de agregar el enlace)*
