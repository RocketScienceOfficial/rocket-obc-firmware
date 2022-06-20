const express = require("express");
const router = express.Router();

router.get("/", function (req, res) {
    console.log("Rendering main website for client");

    res.render("index");
});

router.use(function (req, res, next) {
    res.status(404);
    res.render("404");
});

module.exports = router;