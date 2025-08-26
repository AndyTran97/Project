// ******* DATA LOADING *******
// We took care of that for you
async function loadData() {
  const medalData = [];
  for (let year = 2008; year <= 2024; year += 4) {
    const yearData = await d3.csv(`data/medals_total_${year}.csv`); // Change to d3.csv() instead of d3.json()
    yearData.forEach((element) => {
      element.year = year;
      medalData.push(element);
    });
  }
  const mapData = await d3.json("data/world.json");
  return { medalData, mapData };
}

// ******* STATE MANAGEMENT *******
// This should be all you need, but feel free to add to this if you need to
// communicate across the visualizations
const globalApplicationState = {
  selectedLocations: [],
  medalData: null,
  mapData: null,
  worldMap: null,
  lineChart: null,
  barChart: null,
  groupColor: [{ id: 0, color: "black" }],
  yearSelection: "Total",
  medalSelection: "Total",
};

//******* APPLICATION MOUNTING *******
loadData().then((loadedData) => {
  console.log("Here is the imported data:", loadedData.medalData);

  // Store the loaded data into the globalApplicationState
  globalApplicationState.medalData = loadedData.medalData;
  globalApplicationState.mapData = loadedData.mapData;

  // Creates the view objects with the global state passed in
  const worldMap = new MapVis(globalApplicationState);
  const lineChart = new LineChart(globalApplicationState);
  const barChart = new BarChart(globalApplicationState);

  globalApplicationState.worldMap = worldMap;
  globalApplicationState.lineChart = lineChart;
  globalApplicationState.barChart = barChart;

  // Goes through each element in selectedLocations and removes it
  d3.select("#clear-button").on("click", function () {
    globalApplicationState.selectedLocations.forEach((element) => {
      worldMap.updateSelectedCountries(element);
    });
  });
  d3.select("#medalSelect").on("change", function () {
    globalApplicationState.medalSelection = d3
      .select("#medalSelect")
      .property("value");
    globalApplicationState.worldMap.updateMapVisual();
  });

  d3.select("#yearSelect").on("change", function () {
    globalApplicationState.yearSelection = d3
      .select("#yearSelect")
      .property("value");
    globalApplicationState.worldMap.updateMapVisual();
  });

  // JavaScript to toggle between charts and control yearContainer visibility
  d3.select("#chartSelect").on("change", function () {
    const selectedChart = d3.select("#chartSelect").property("value");

    if (selectedChart === "bar") {
      // Show Bar Chart, hide Line Chart
      d3.select("#bar-chart-container").style("display", "block");
      d3.select("#line-chart-container").style("display", "none");

      // Update Bar Chart with current state
      globalApplicationState.barChart.updateBarChart();
    } else {
      // Show Line Chart, hide Bar Chart
      d3.select("#line-chart-container").style("display", "block");
      d3.select("#bar-chart-container").style("display", "none");

      // Update Line Chart with current state
      globalApplicationState.lineChart.updateLineChart();
    }
  });
});
