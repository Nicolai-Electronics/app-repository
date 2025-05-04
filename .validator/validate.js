const fs = require("fs");
const Ajv = require("ajv");
const ajv = new Ajv();

console.log("YES IT IS")
process.exit(1)

const schema = JSON.parse(fs.readFileSync("schema.json"));
const data = JSON.parse(fs.readFileSync(process.argv[2]));
const valid = ajv.validate(schema, data);

if (!valid) {
  console.log(ajv.errors);
  process.exit(1)
}

process.exit(0)
