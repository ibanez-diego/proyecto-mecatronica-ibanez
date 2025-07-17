import serial
import time
import cv2
from ultralytics import YOLO

# -------------------- BLOQUE 1: Comunicación con ESP32 --------------------
class ESP32Control:
    def __init__(self, port='COM4', baudrate=115200):
        self.esp32 = serial.Serial(port, baudrate, timeout=1)
        time.sleep(2)

    def activar_rele(self):
        self.esp32.write(b'R')
        print("Relé activado (cinta avanzando).")

    def desactivar_rele(self):
        self.esp32.write(b'O')
        print("Relé desactivado (cinta detenida).")

    def ejecutar_rutina_inspeccion(self):
        self.esp32.write(b'S')
        print("Rutina de inspección enviada al ESP32.")

    def cancelar_inspeccion(self):
        self.esp32.write(b'C')
        print("Inspección cancelada. Motores volverán a inicio.")

    def cerrar(self):
        self.esp32.close()
        print("Conexión cerrada con ESP32.")

# -------------------- BLOQUE 2: YOLOv8 - Detección y Falla --------------------
class VisionArtificial:
    def __init__(self, modelo_path='best.pt'):
        self.modelo = YOLO(modelo_path)
        self.cam = cv2.VideoCapture(0)

    def detectar(self):
        ret, frame = self.cam.read()
        if not ret:
            return False, False, frame

        resultados = self.modelo.predict(source=frame, verbose=False, conf=0.5)

        engranaje_detectado = False
        defecto_detectado = False

        for result in resultados:
            for cls in result.boxes.cls:
                clase = result.names[int(cls)]
                if clase == 'engranaje':
                    engranaje_detectado = True
                if clase == 'defecto':
                    defecto_detectado = True

        return engranaje_detectado, defecto_detectado, frame

    def liberar(self):
        self.cam.release()
        cv2.destroyAllWindows()

# -------------------- BLOQUE 3: Comunicación con SCARA --------------------
class SCARAControl:
    def __init__(self, port='COM6', baudrate=115200):
        self.arduino = serial.Serial(port, baudrate, timeout=1)
        time.sleep(2)

    def preparar_scara(self):
        print("Enviando homing al SCARA...")
        self.arduino.write(b'H')
        self.esperar_ok()

    def ejecutar_rutina_scara(self):
        print("Ejecutando rutina SCARA...")
        self.arduino.write(b'R')
        self.esperar_ok()
        print("Rutina SCARA finalizada.")

    def cerrar(self):
        self.arduino.close()
        print("Conexión cerrada con SCARA.")

    def esperar_ok(self):
        while True:
            if self.arduino.in_waiting > 0:
                respuesta = self.arduino.readline().decode().strip()
                if respuesta == "OK":
                    print("SCARA confirmó OK.")
                    break

# -------------------- BLOQUE 4: Lógica Principal --------------------
def main():
    esp = ESP32Control(port='COM4')
    scara = SCARAControl(port='COM6')
    vision = VisionArtificial('best.pt')

    scara.preparar_scara()  # Homing inicial

    activar_scara = False

    try:
        while True:
            print("\n--- Menú de Control ---")
            print("1: Avanzar cinta")
            print("2: Detener cinta")
            print("3: Realizar inspección")
            print("q: Salir")
            comando = input("Ingrese la opción: ")

            if comando == '1':
                esp.activar_rele()

            elif comando == '2':
                esp.desactivar_rele()
                if activar_scara:
                    scara.ejecutar_rutina_scara()
                    activar_scara = False
                else:
                    print("Cinta detenida. No hay defecto detectado.")

            elif comando == '3':
                esp.desactivar_rele()
                print("Iniciando inspección...")
                esp.ejecutar_rutina_inspeccion()

                defecto_detectado = False

                while True:
                    engranaje_detectado, defecto_detectado_actual, frame = vision.detectar()

                    if defecto_detectado_actual:
                        defecto_detectado = True
                        cv2.putText(frame, "DEFECTO DETECTADO", (30, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 3)
                        esp.cancelar_inspeccion()
                        break

                    cv2.imshow('Inspección', frame)
                    if cv2.waitKey(1) & 0xFF == ord('q'):
                        print("Inspección cancelada por el usuario.")
                        esp.cerrar()
                        scara.cerrar()
                        vision.liberar()
                        return

                    if esp.esp32.in_waiting > 0:
                        respuesta = esp.esp32.readline().decode().strip()
                        if respuesta == "OK":
                            print("ESP32 confirmó fin de rutina de inspección.")
                            break

                cv2.destroyAllWindows()

                if defecto_detectado:
                    print("Defecto detectado. La cinta se activará desde el ESP32.")
                    activar_scara = True
                else:
                    print("No se detectó defecto. La cinta se activará desde el ESP32.")

            elif comando == 'q':
                print("Finalizando programa y cerrando conexiones...")
                esp.cerrar()
                scara.cerrar()
                vision.liberar()
                break

            else:
                print("Comando inválido. Intente de nuevo.")

    except KeyboardInterrupt:
        print("Programa interrumpido manualmente.")
        esp.cerrar()
        scara.cerrar()
        vision.liberar()

if __name__ == "__main__":
    main()
