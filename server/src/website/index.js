const express = require("express");
const router = express.Router();

router.get("/", function (req, res) {
    console.log("Rendering main website for client");

    res.render("index");
});

module.exports = router;