TCP = 0
UDP = 1

create_conf_table = '''CREATE TABLE Conf (
    ProtocolId INTEGER NOT NULL,
    TransportLayer INTEGER NOT NULL
);'''

create_data_table = '''CREATE TABLE Data (
    DeviceId INTEGER NOT NULL,
    MAC TEXT NOT NULL,
    Val INTEGER NOT NULL,
    BattLevel INTEGER NOT NULL,
    Timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    Temp INTEGER,
    Press INTEGER,
    Hum FLOAT,
    Co FLOAT,
    RMS FLOAT,
    AmpX FLOAT,
    FrecX FLOAT,
    AmpY FLOAT,
    FrecY FLOAT,
    AmpZ FLOAT,
    FrecZ FLOAT,
    AccX TEXT,
    AccY TEXT,
    AccZ TEXT
);'''

create_logs_table = '''CREATE TABLE Logs (
    DeviceId INTEGER NOT NULL,
    TransportLayer INTEGER NOT NULL,
    ProtocolId INTEGER NOT NULL,
    Timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
);'''


create_loss_table = '''CREATE TABLE Loss (
    Latency INTEGER NOT NULL,
    PacketLoss INTEGER NOT NULL
);'''

create_tables = [create_conf_table, create_data_table, create_logs_table, create_loss_table]

import sqlite3 as sql


conn = sql.connect("DB.sqlite")
cur = conn.cursor()

# crea cada tabla
for create_table in create_tables:
    cur.execute(create_table)

# inserta la única row de conf con la configuración inicial (protocol_id = 1, transport_layer = 0 o tcp)
cur.execute("INSERT INTO Conf (ProtocolId, TransportLayer) values (?, ?)", (1, TCP))

conn.close()

# inicializa la BDD
