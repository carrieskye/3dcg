const fs = require('fs');
const es = require('event-stream');

const mesh = 'bunny';
const trianglesPerBranch = 10;
const boxesPerBranch = 8;
const readFilename = `./samples/${mesh}.mesh`;
//const writeFilename = `./results/${mesh}-${trianglesPerBranch}.mesh`;
const writeFilename = `D:/meshes/larry/${mesh}-${trianglesPerBranch}.mesh`;

// helper function to check current memory usage
const checkMemory = () => {
  const used = process.memoryUsage().heapUsed / 1024 / 1024;
  console.log(`  used approximately ${Math.round(used * 100) / 100} MB memory`);
};

// helpers to check time and memory for a block
const measure = (key, description) => {
  console.log(description)
  console.time(key);
};
const measureEnd = (key) => {
  console.timeEnd(key);
  checkMemory();
};

const readable3d = { 0: 'x', 1: 'y', 2: 'z' };
const readStream = fs.createReadStream(readFilename);

// vertices, is freed after reading triangles
let vertices = [];

/**
 * Array of triangles: [
 *  [ [1,2,3], [0,0,1], [1,3,4] ],
 *  ...
 * ]
 */
let triangles = [];
const populateVertices = (vertexIndex) => vertices[vertexIndex];

// bouding box, will be determined during read
const bounding_box = {
  min: [ Infinity, Infinity, Infinity, ],
  max: [ -Infinity, -Infinity, -Infinity, ]
};
const check_bouding_box = (box, vertex) => {
  [0, 1, 2].forEach(i => {
    if (box.max[i] < vertex[i]) box.max[i] = vertex[i];
    if (box.min[i] > vertex[i]) box.min[i] = vertex[i];
  });
}
/**
 * 0: amount vertices
 * 1: vertices
 * 2: amount triangles
 * 3: triangles
 */
let reading = 0;
let currentIndex = 0;
let amountVertices = 0;
let amountTriangles = 0;
readStream.pipe(es.split()).pipe(es.mapSync(line => {
  // pause readStream
  readStream.pause();

  switch (reading) {
    case 0: // read amount vertices
      amountVertices = parseInt(line);
      reading++;
      console.log("reading all vertices");
      console.time("read-vertices");
      break;

    case 1: // read vertices
      //console.log("vertex", currentIndex, line);
      let vertex = line.split(' ').map(coordinate => parseFloat(coordinate));

      check_bouding_box(bounding_box, vertex);

      vertices.push(vertex);
      if (currentIndex + 1 == amountVertices) {
        reading++;
        console.timeEnd("read-vertices");
        checkMemory();
      }
      currentIndex++;
      break;

    case 2: // read amount triangles
      amountTriangles = parseInt(line);
      reading++;
      console.log("reading all triangles");
      console.time("read-triangles");
      break;

    case 3: // read triangles
    let vs = line.split(' ')
          .map(str => parseInt(str)); // convert str to int
      if (vs.length != 3) {
        break;
      }
      triangles.push(vs);
      break;
  }
  // resume
  readStream.resume();
})
).on("error", err => console.log("Error while reading file.", err)).on("end", () => {
  let vertices_length = vertices.length;
  //vertices = null;
  //if (global.gc) { global.gc(); console.info('ran garbage collector'); }
  console.timeEnd("read-triangles");
  console.log(`  Read ${vertices_length} vertices and ${triangles.length} triangles.`);
  console.log('  Bouding Box', bounding_box);
  checkMemory();

  measure('triangles-avg', 'Calculating averages');
  let triangles_with_avg = triangles.map(triangle => triangle.concat([
    [
      triangle.map(populateVertices).map(coordinates => coordinates[0]).reduce((p,c) => p+c, 0)/3,
      triangle.map(populateVertices).map(coordinates => coordinates[1]).reduce((p,c) => p+c, 0)/3,
      triangle.map(populateVertices).map(coordinates => coordinates[2]).reduce((p,c) => p+c, 0)/3,
    ]
  ]));
  measureEnd('triangles-avg');
  triangles = null;
  checkMemory();
  if (global.gc) { global.gc(); console.info('ran garbage collector'); }
  checkMemory();


  //console.log(triangles_with_avg);
  measure('triangles-sort', 'Sorting by distance from first');
  const bouding_box_dimentions = bounding_box.max.map((c, i) => c - bounding_box.min[i]);
  console.log(bouding_box_dimentions);
  let maxDir = 0;
  bouding_box_dimentions.forEach((width, i) => {
    if (bouding_box_dimentions[maxDir] < width) maxDir = i;
  });
  console.log('max dim is', readable3d[maxDir], 'and its distance is', bouding_box_dimentions[maxDir]);
  //const origin = triangles_with_avg[0][3];
  //const origin = bounding_box.min;
  triangles_with_avg.sort((a, b) => {
    //let distance_a = distance3d(origin, a[3]);
    //let distance_b = distance3d(origin, b[3]);
    //console.log('a', distance_a);
    //console.log('b', distance_b);
    //console.log(b[3][maxDir], a[3][maxDir], b[3][maxDir] - a[3][maxDir]);
    return b[3][maxDir] - a[3][maxDir];
  });
  measureEnd('triangles-sort');

  measure('build-tree', 'Building tree');
  let i = 0;
  let boxes = [];
  let boxIndex = 0;
  while (i < triangles_with_avg.length - trianglesPerBranch) {
    let box = { index: boxIndex++, children: [], triangles: [] };
    for (let j = 0; j < trianglesPerBranch; j++) {
      box.triangles.push(j+i);
    }
    i += trianglesPerBranch;
    boxes.push(box);
  }
  let box = { index: boxIndex++, children: [], triangles: [] };
  for (j = i; j < triangles_with_avg.length; j++) {
    box.triangles.push(j);
  }
  boxes.push(box);



  const groupBoxes = (boxes) => {
    let i = 0;
    const newboxes = [];
    while (i < boxes.length - boxesPerBranch) {
      let box = { index: boxIndex++, children: [], triangles: [] };
      for (let j = 0; j < boxesPerBranch; j++) {
        box.children.push(boxes[j + i]);
      }
      i += boxesPerBranch;
      newboxes.push(box);
    };
    let box = { index: boxIndex++, children: [], triangles: [] };
    for (j = i; j < boxes.length; j++) {
      box.children.push(boxes[j]);
    }
    newboxes.push(box);
    if (newboxes.length <= 3) 
      return newboxes;
    return groupBoxes(newboxes);
  }
  let rootBox = { index: boxIndex++, children: groupBoxes(boxes), triangles: [] };


  /*
  let currentNode = {
    index: 0,
    triangles: [],
    children: [],
    bounding_box: {
      min: [ Infinity, Infinity, Infinity, ],
      max: [ -Infinity, -Infinity, -Infinity, ]
    }
  };
  for(let i = 0; i < triangles_with_avg.length; i++) {
    if (currentNode.triangles.length >= trianglesPerBranch) {
      currentNode = {
        index: currentNode.index + 1,
        triangles: [],
        children: [currentNode],
        bounding_box: currentNode.bounding_box
      }
    }
    currentNode.triangles.push(i);
    //console.log('we are pusing', i, 'to', currentNode.index)
    check_bouding_box(currentNode.bounding_box, vertices[triangles_with_avg[i][0]])
    check_bouding_box(currentNode.bounding_box, vertices[triangles_with_avg[i][1]])
    check_bouding_box(currentNode.bounding_box, vertices[triangles_with_avg[i][2]])
  }
  */
  measureEnd('build-tree');

  //console.log(currentNode);

  //printTree(currentNode);

  measure('write-tree', 'Write tree to file');
  const writeStream = fs.createWriteStream(writeFilename);
  const write = (content) => writeStream.write(content + '\n');
  write('Vertices');
  let vertices_offset = 2;
  write(vertices.map((v, i) => `${vertices_offset+i} ${v.join(';')}`).join("\n"));
  write('Triangles');
  let triangleOffset = vertices_offset + vertices.length + 1; 
  write(triangles_with_avg.map((v, i) => `${triangleOffset+i} ${v.filter((c, vi) => vi < 3).map(vi => vi + vertices_offset).join(';')}`).join("\n"));
  write('Boxes');
  let boxOffset = triangleOffset + triangles_with_avg.length + 1; 
  const printb = (node) => {
    //console.log('tringles:', node.triangles.map(t => t.filter((c,i) => i < 3).join(';')));
    write(
      (node.index + boxOffset) + ' ' +
      node.triangles.map(i => triangleOffset + i).concat( 
        node.children.map(child => child.index + boxOffset)
      ).join(';')
    );
    for (let i = 0; i < node.children.length; i++)
      printb(node.children[i]);
  }
  printb(rootBox);
  //printTree(currentNode);

  measureEnd('write-tree');
});


const printTree = (node, depth) => {
  if (typeof depth === 'undefined') depth = 0;
  console.log(' '.repeat(depth) + 'N(' + node.index + ')');
  console.log(' '.repeat(depth) + 'T ' + node.triangles.length);
  console.log(' '.repeat(depth) + 'C ' + node.children.length);
  node.children.forEach(child => printTree(child, depth+1));
}

const distance3d = (p1, p2) => Math.sqrt(
  Math.pow(p2[0] - p1[0], 2),
  Math.pow(p2[1] - p1[1], 2),
  Math.pow(p2[2] - p1[2], 2)
);