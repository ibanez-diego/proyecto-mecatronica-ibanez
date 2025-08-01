# Proyecto: Sistema Robótico de Inspección con Visión Artificial y Brazo SCARA

## 🚀 Descripción General
Este proyecto consiste en un sistema automático de inspección de piezas mediante Visión Artificial (YOLOv8) y un brazo robótico SCARA, orientado a automatizar procesos de control de calidad en líneas de producción. Todo el sistema es controlado desde Python, integrando un ESP32 para la cinta transportadora y un Arduino Mega para el SCARA.

---

## 🎯 Objetivo
Automatizar la detección de defectos en piezas (engranajes) y realizar su clasificación mediante un brazo SCARA, reduciendo tiempos de inspección manual y mejorando la trazabilidad del proceso.

---

## 🛠️ Componentes Principales
### Hardware
- ESP32 (Control de cinta transportadora y secuencia de inspección)
- Arduino Uno + CNC Shield V3 (Control del SCARA)
- Brazo Robótico SCARA 4 DOF
- Motor paso a paso NEMA 17 + Drivers A4988
- Servo MG996R (Gripper SCARA)
- Relé 5V (Control cinta)
- Cinta transportadora
- Cámara USB

### Software
- YOLOv8 (detección de defectos)
- Python (Control general e interfaz)
- Arduino IDE (Código SCARA y ESP32)
- Processing (Calibración de posiciones SCARA)

---

## 🗂️ Estructura del Repositorio
- `Documentation/` → Documentos, propuesta inicial
- `Hardware/` →  esquemas de conexión
- `Media/`
  - `fotos/` → Imágenes del prototipo
  - `videos/` → Videos del funcionamiento
- `Software/` → Código fuente Python, Arduino y modelos de IA
- `README.md` → (Este archivo)

---

## 🕒 Cronograma y Tiempos
| Tarea                                | Estimado | Real   | Desvío |
|--------------------------------------|----------|--------|--------|
| Diseño mecánico inicial              | 15 hs    | 18 hs  | +3 hs  |
| Diseño electrónico y cableado        | 10 hs    | 25 hs  | +15 hs  |
| Desarrollo de software (Python)      | 20 hs    | 35 hs  | +15 hs  |
| Integración de hardware              | 15 hs    | 15 hs  | 0 hs   |
| Entrenamiento YOLOv8                 | 8 hs     | 8 hs   | 0 hs   |
| Testeo y depuración                  | 12 hs    | 20 hs  | +8 hs  |

---

## 💸 Costos
| Ítem                         | Estimado | Real     | Desvío   |
|------------------------------|----------|----------|----------|
| Materiales mecánicos          | $40.000  | $50.000  | +$10.000  |
| Electrónica y sensores        | $70.000  | $85.000  | +$15.000  |
| Costo impresión 3D            | $15.000  | $15.000  | $0       |
| Total                        | $125.000  | $150.000  | +$15.000  |

Costo estimado proyecto real: $500.000 (orden de magnitud)
Costo operativo: Bajo (solo consumo eléctrico y mantenimiento preventivo)

---

## 📐 Cálculos
- **Mecánicos**:  límites de carrera SCARA, velocidad máxima segura de operación.
- **Eléctricos**: Consumo total 5A @12V, dimensionado de fuente, cálculo de disipación en A4988, corrientes en pines del ESP32 (máx 12mA)

---

## 📄 Planos y Esquemas
- Planos en Fusion360 (.step)
- Esquema de conexionado de ESP32, relé y servo.
- Esquema de conexionado SCARA (drivers, motores, límites de carrera).

---

## 🧪 Ensayos y Pruebas
- Pruebas con engranajes con y sin defectos
- Test de rutina SCARA (20 ciclos sin error)
- Prueba de parada de emergencia desde Python
- Prueba de desvío de piezas defectuosas

---

## 📚 Manual de Usuario
1. Encender fuente de alimentación (12V)
2. Ejecutar el script Python principal
3. Seleccionar “Iniciar Ciclo” en el menú
4. El sistema realizará inspección y clasificación automáticamente
5. Salir con opción 'q'

---

## 🔮 Futuras Mejoras
- Agregar base de datos para trazabilidad
- Implementar detección multi-clase (más defectos)
- Interfaz gráfica amigable para operario
- Añadir feedback visual (pantalla de estado)

---

## 📸 Media
Videos de demostración:
- [Engranaje sin defecto](https://youtube.com/shorts/1ZNBOIqG2KE)
- [Engranaje con defecto](https://youtube.com/shorts/WU-7bY1HUUE)

---

## 📋 Consultas a Docentes
Constancia de consultas realizadas a docentes de Robótica y Electrónica (Adjunto en carpeta Documentation)

---

## 🤖 Proyecto realizado por: Diego Ibañez
Facultad de Ingeniería, UNLZ
Ingeniería Mecatrónica

---

"Automatizar la calidad, simplificar el proceso."

