import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    stocks = db.execute("SELECT * FROM shareT WHERE Link = :link", link= session["user_id"])
    cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
    price = list()
    t1 = list()
    t2 = list()
    total = cash[0]["cash"]
    for row in stocks:
        price.append(lookup(row["Stock"]))
        A = price[len(price)-1]
        t1.append(usd(A["price"]))
        t2.append(usd(A["price"]*row["Shares"]))
        total+=A["price"]*row["Shares"]

    """Show portfolio of stocks"""
    return render_template("index.html",cash=usd(cash[0]["cash"]),\
    stocks=stocks,price= price, t1 = t1,t2=t2, total=usd(total))  # ---------->> must pass cash, shares, prices


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():

    if request.method == "POST":

        cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
        pri = lookup(request.form.get("symbol"))
        st = request.form.get("shares")

        if not pri:
            return apology("Stock Not Found")

        elif not st or not st.isdigit() or float(st)<0:
            return apology("Input shares")

        else:
            try:
                int(st)
            except ValueError:
                return apology("")

            if cash[0]["cash"] < float(st)*pri["price"]:
                return apology("not enough cash")

            result = db.execute("INSERT INTO 'portfolio' (Link, Shares, Stock, Price, Total)\
            VALUES(:id, :share, :stock, :price, :total)",\
            id= session["user_id"],share= st, stock= pri["symbol"],\
            price= pri["price"], total = float(st)*pri["price"])

            newCash = cash[0]["cash"] -float(st)*pri["price"]
            db.execute("UPDATE 'users' SET cash = :cash WHERE id = :id", cash = newCash, id = session["user_id"])

            if not result:
                return apology("problem")
            else:

                update = db.execute("SELECT * FROM 'shareT' WHERE Link = :Link AND Stock= :stock"\
                , Link= session["user_id"], stock = pri["symbol"])


                if len(update) != 1:
                    db.execute("INSERT INTO 'shareT' (Link,Stock, Shares) VALUES(:Link, :Stock, :Shares)",\
                    Link=session["user_id"], Stock= pri["symbol"], Shares= st)
                else:
                    totalS = update[0]["Shares"]+int(st)
                    db.execute("UPDATE 'shareT' SET Shares = :total WHERE prime= :prime",total= totalS, prime=update[0]["prime"])
                return redirect("/")

    return render_template("buy.html")



@app.route("/history")
@login_required                                                                             #send rows of information to create table send dat
def history():
    result = db.execute("SELECT * FROM portfolio WHERE Link=:link", link=session["user_id"])

    return render_template("history.html", result = result)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":

        quote = lookup(request.form.get("symbol"))
        """Get stock quote."""
        if not quote:
            return apology("Stock Not Found")
        else:
            return render_template("display.html", company=quote["name"], stock=quote["symbol"], cost="{0:.2f}".format(quote["price"]))

    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():

    if request.method == "POST":

        if not request.form.get("username"):
            return apology("Provide Username", 400)

        elif not request.form.get("password"):
            return apology("provide password", 400)

        elif not request.form.get("confirmation"):
            return apology("provide password(again)", 400)

        elif request.form.get("confirmation") != request.form.get("password"):
            return apology("Passwords do not match", 400)

        else:
            result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)",
            username=request.form.get("username"),hash= generate_password_hash(request.form.get("password")))

            if not result:
                return apology("Username already in use",400)
            else:
                rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))
                session["user_id"] = rows[0]["id"]
                return redirect("/")

    """Register user"""
    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():                                 #---->  check shares


    dropDown = db.execute("SELECT Stock FROM shareT WHERE Link= :Link",Link=session["user_id"])
    if request.method == "POST":
        result = db.execute("SELECT * FROM shareT WHERE Link=:link AND Stock= :stock",
                             link= session["user_id"], stock = request.form.get("symbol"))
        if not result:
            return apology("Stock not in portfolio")
        elif result[0]["Shares"] < int(request.form.get("shares")):
            return apology("Not enough shares")
        else:
            if result[0]["Shares"] == int(request.form.get("shares")):
                db.execute("DELETE FROM shareT WHERE prime =:prime", prime= result[0]["prime"])
            else:
                db.execute("UPDATE 'shareT' SET Shares= :share WHERE prime = :prime"
                            , share= result[0]["Shares"]-int(request.form.get("shares"))
                            ,prime = result[0]["prime"])

            pri = lookup(request.form.get("symbol"))["price"]

            cash = db.execute("SELECT cash FROM users WHERE id=:id",id=session["user_id"])
            newCash = cash[0]["cash"]+int(request.form.get("shares"))*pri
            db.execute("UPDATE 'users' SET cash = :cash WHERE id = :id", cash = newCash, id = session["user_id"])

            result = db.execute("INSERT INTO 'portfolio' (Link, Shares, Stock, Price, Total)\
                        VALUES(:id, :share, :stock, :price, :total)",\
                        id= session["user_id"],share= -int(request.form.get("shares"))\
                        , stock= request.form.get("symbol"),\
                        price= pri, total = float(request.form.get("shares"))*pri)


            return redirect("/")

    return render_template("sell.html", drop = dropDown)


@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addcash():
    if request.method == "POST":

        cash = request.form.get("cash")

        if not cash or not cash.isdigit() or float(cash)<0:
            return apology("ERROR")
        else:
            c2 = db.execute("SELECT cash FROM users WHERE id=:id",id=session["user_id"])
            db.execute("UPDATE 'users' SET cash = :cash WHERE id= :id",cash= c2[0]["cash"]+float(cash), id=session["user_id"])
            return redirect("/")

    return render_template("addcash.html")



def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
