const fs = require("fs");
const path = require("path");
const Ajv = require("ajv");
const ajv = new Ajv();

const schema = JSON.parse(fs.readFileSync("schema.json"));
const metadataPath = process.argv[2];
const data = JSON.parse(fs.readFileSync(metadataPath));
const valid = ajv.validate(schema, data);

if (!valid) {
  console.log(ajv.errors);
  process.exit(1)
}

const appDir = path.dirname(metadataPath);
const missingAssets = [];

data.application.forEach((application, applicationIndex) => {
  (application.assets || []).forEach((asset) => {
    const assetPath = path.join(appDir, asset.source_file);
    if (!fs.existsSync(assetPath)) {
      missingAssets.push({
        application: applicationIndex,
        targets: application.targets,
        source_file: asset.source_file,
      });
    }
  });
});

if (missingAssets.length > 0) {
  console.log("Missing asset files:", missingAssets);
  process.exit(1)
}

process.exit(0)
