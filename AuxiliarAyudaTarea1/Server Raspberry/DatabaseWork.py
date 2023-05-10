import datetime
import sqlite3 as sql

# Documentación https://docs.python.org/3/library/sqlite3.html

def save_log(header, data):
    with sql.connect("DB.sqlite") as con:
        cur = con.cursor()
        cur.execute(
            "INSERT INTO Logs (DeviceId, TransportLayer, ProtocolId, Timestamp) VALUES (?, ?, ?, ?)",
            (header["device_id"], header["transport_layer"], header["protocol_id"], data["timestamp"])
        )

def save_loss(header, data, data_length):
    with sql.connect("DB.sqlite") as con:
        cur = con.cursor()
        latency = datetime.datetime.now() - data["timestamp"]
        packet_loss = header["length"] - data_length
        cur.execute(
            "INSERT INTO Loss (Latency, PacketLoss) VALUES (?, ?)",
            (latency, packet_loss)
            )

def save_data(header, data):
    with sql.connect("DB.sqlite") as con:
        protocol_0_params = (header["device_id"], header["MAC"], data["val"], data["batt_level"], data["timestamp"])
        protocol_1_params = protocol_0_params + (data["temp"], data["press"], data["hum"], data["co"])
        protocol_2_params = protocol_1_params + (data["RMS"],)
        protocol_3_params = protocol_2_params + (data["amp_x"], data["frec_x"], data["amp_y"], data["frec_y"], data["amp_z"], data["frec_z"])
        protocol_4_params = protocol_1_params + (data["acc_x"], data["acc_y"], data["acc_z"])

        params = [protocol_0_params, protocol_1_params, protocol_2_params, protocol_3_params, protocol_4_params]

        queries = [
            "INSERT INTO Data (DeviceId, MAC, Val, BattLevel, Timestamp) VALUES (?, ?, ?, ?, ?)",
            "INSERT INTO Data (DeviceId, MAC, Val, BattLevel, Timestamp, Temp, Press, Hum, Co) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)",
            "INSERT INTO Data (DeviceId, MAC, Val, BattLevel, Timestamp, Temp, Press, Hum, Co, RMS) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
            "INSERT INTO Data (DeviceId, MAC, Val, BattLevel, Timestamp, Temp, Press, Hum, Co, RMS, AmpX, FrecX, AmpY, FreqY, AmpZ, FreqZ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
            "INSERT INTO Data (DeviceId, MAC, Val, BattLevel, Timestamp, Temp, Press, Hum, Co, AccX, AccY, AccZ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"
        ]
        cur = con.cursor()
        cur.execute(
            queries[header["protocol"]],
            params[header["protocol"]]
        )

def read_conf():
    with sql.connect("DB.sqlite") as con:
        cur = con.cursor()
        res = cur.execute(
            "SELECT * FROM Conf WHERE rowid = 1"
        )

        return res

def update_conf(protocol_id, transport_layer):
    with sql.connect("DB.sqlite") as con:
        cur = con.cursor()
        cur.execute(
            '''UPDATE Conf
            SET ProtocolId = ?,
                TransportLayer = ?
            WHERE rowid = 1
            ''',
            (protocol_id, transport_layer)
        )
