'''
    HTTP 4db igéjével db módosítás legyen
    CURL-el használni

'''
from flask import Flask, request, jsonify
from flask_sqlalchemy import SQLAlchemy
from flask_marshmallow import Marshmallow
import os
import datetime

app = Flask(__name__)
basedir = os.path.abspath(os.path.dirname(__file__))
app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql+pymysql://test:testpasswd2@52.59.59.105/test2'
db = SQLAlchemy(app)
ma = Marshmallow(app)


class testtable(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    lm35 = db.Column(db.Float, unique=True)
    dht22 = db.Column(db.Integer, unique=True)
    datetime = db.Column(db.DateTime, unique=True)

    def __init__(self, dht22, lm35):
        self.dht22 = dht22
        self.lm35 = lm35
        self.datetime = datetime.datetime.now()


class testtableSchema(ma.Schema):
    class Meta:
        # Fields to expose
        fields = ('id','dht22', 'lm35', 'datetime')


data_schema = testtableSchema()
datas_schema = testtableSchema(many=True)


# endpoint to create new user
@app.route("/data", methods=["POST"])
def add_user():
    lm35 = request.json['lm35']
    dht22 = request.json['dht22']
    
    new_data = testtable(lm35, dht22)

    db.session.add(new_data)
    db.session.commit()

    return "{'data': 'OK'}"


# endpoint to show all users
@app.route("/data", methods=["GET"])
def get_user():
    all_users = testtable.query.all()
    result = datas_schema.dump(all_users)
    return jsonify(result.data)


# endpoint to get user detail by id
@app.route("/data/<id>", methods=["GET"])
def user_detail(id):
    user = testtable.query.get(id)
    return data_schema.jsonify(user)


# endpoint to update user
@app.route("/data/<id>", methods=["PUT"])
def user_update(id):
    tdata = testtable.query.get(id)
    nlm35 = request.json['lm35']
    ndht22 = request.json['dht22']
    tdata.lm35 = nlm35
    tdata.dht22 = ndht22
    tdata.datetime = datetime.datetime.now()
    db.session.commit()
    return data_schema.jsonify(tdata)


# endpoint to delete user
@app.route("/data/<id>", methods=["DELETE"])
def user_delete(id):
    tdata = testtable.query.get(id)
    db.session.delete(tdata)
    db.session.commit()

    return data_schema.jsonify(tdata)


if __name__ == '__main__':
    app.run(debug=True, port=5001)