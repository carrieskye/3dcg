
const triangles = [];

for (let i = 0; i < 10; i++) triangles.push(Math.round(Math.random()*100));

//console.log(triangles);

triangles.sort();

console.log(triangles);

const boxes = []

let n = 2;

let i = 0;
let boxIndex = 0;
while (i < triangles.length - n) {
  let box = { index: boxIndex++, children: [] };
  for (let j = 0; j < n; j++) {
    box.children.push(triangles[j+i]);
  }
  i += n;
  boxes.push(box);
};
let box = [];
for (j = i; j < triangles.length; j++) {
  box.push(triangles[j]);
}
boxes.push(box);

console.log(boxes);




const groupBoxes = (boxes) => {
  let n = 3;
  let i = 0;
  const newboxes = [];
  while (i < boxes.length - n) {
    let box = [];
    for (let j = 0; j < n; j++) {
      box.push(boxes[j + i]);
    }
    i += n;
    newboxes.push(box);
  };
  box = [];
  for (j = i; j < boxes.length; j++) {
    box.push(boxes[j]);
  }
  newboxes.push(box);
  if (newboxes.length <= 3) 
    return newboxes;
  return groupBoxes(newboxes);
}
const r = groupBoxes(boxes);
console.log(r, r.length);
