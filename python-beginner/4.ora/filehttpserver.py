
import os
from flask import Flask,flash,request,redirect,url_for
from flask import send_from_directory
from werkzeug.utils import secure_filename
from flask import jsonify

UPLOAD_FOLDER = "/home/nolex/git/netacademia/python-beginner/4.ora/upload"
ALLOWED_EXTENSIONS = set(['txt', 'csv'])

app=Flask(__name__)
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

def allowed_file(filename):
    return '.' in filename and filename.rsplit('.',1)[1].lower() in ALLOWED_EXTENSIONS

@app.route('/', methods=['POST', 'GET'])
def upload_file():
    if request.method == 'POST':
        print("post request")
        # check if the post request has the file part
        if 'file' not in request.files:
            flash('No file part')
            print("asd")
            return redirect(request.url)
        file = request.files['file']
        print(file)
        # if user does not select file, browser also
        # submit an empty part without filename
        if file.filename == '':
            flash('No selected file')
            print("qwe")
            return redirect(request.url)
        if file and allowed_file(file.filename):
            print("xcv")
            filename = secure_filename(file.filename)
            file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
            #return redirect(url_for('uploaded_file',filename=filename))
            rd = {}
            rd['file'] = filename
            return jsonify(rd)
    print("other request")
    return '''
    <!doctype html>
    <title>Upload new File</title>
    <h1>Upload new File</h1>
    <form method=post enctype=multipart/form-data>
    <input type=file name=file>
    <input type=submit value=Upload>
    </form>
    '''
    

@app.route('/uploads/<filename>')
def uploaded_file(filename):
    return send_from_directory(app.config['UPLOAD_FOLDER'], filename)