from flask import Flask
from flask import request
from markupsafe import escape

app = Flask(__name__)

@app.route("")
def index():
    return "Index Page"