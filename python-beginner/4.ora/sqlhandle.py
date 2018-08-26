

from sqlalchemy import create_engine

dbcon = create_engine("mysql+pymysql://test:testpasswd2@52.59.59.105/test2")

con = dbcon.connect()

query = con.execute("select * from test2.testtable;")

result = query.cursor.fetchall()
print(str(result[0][3]))