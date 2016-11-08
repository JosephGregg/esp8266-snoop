// obviously this is a test. 
//TODO: hook in to database, sanitize input received, send redirect if chip already exists in db
<?PHP
if($_POST["chip_id"] == "snip") {
echo "true\n";
header("HTTP/1.1 202");
} else {
echo "false\n";
}
?>
