const express = require("express");
const serialPortControl = require("./src/serialPortControl");

const app = express();
const PORT = 8080;

serialPortControl.listen(data => console.log(data));

app.use(express.static("public"));
app.use("/css", express.static(__dirname + "public/css"));
app.use("/img", express.static(__dirname + "public/img"));
app.use("/js", express.static(__dirname + "public/js"));

app.set("views", "./views");
app.set("view engine", "ejs");

app.get("/", function (req, res) {
    res.render("index");
});

app.listen(PORT, () => console.log(`Listening on port ${PORT}`));