/** Class representing the map view. */
class MapVis {
  /**
   * Creates a Map Visualization
   * @param globalApplicationState The shared global application state (has the data and the line chart instance in it)
   */
  constructor(globalApplicationState) {
    this.globalApplicationState = globalApplicationState;

    // Set up the map projection
    const projection = d3.geoWinkel3().scale(110).translate([325, 250]);
    this.path = d3.geoPath().projection(projection);
    const json = topojson.feature(
      globalApplicationState.mapData,
      globalApplicationState.mapData.objects.countries
    );

    let medalData = this.getSelectedMedalData(globalApplicationState.medalData);

    // Initialize color scale
    const color = d3
      .scaleQuantize()
      .range([
        "#fff7ee",
        "#ffd6ca",
        "#ffaf96",
        "#fb8260",
        "#f44f39",
        "#d52220",
        "#ab1115",
        "#67010f",
      ])
      .domain(d3.extent(medalData, (d) => +d.totalCount));

    // Create a lookup of maximum total medal count
    const dataLookup = d3.rollup(
      medalData,
      (values) => d3.max(values, (d) => +d.totalCount),
      (d) => d.country_code
    );

    // Set country value for each feature in json
    json.features.forEach((feature) => {
      feature.value = dataLookup.get(feature.id) || 0;
    });

    // Draw the map
    d3.select("#countries")
      .selectAll("path")
      .data(json.features)
      .join("path")
      .attr("id", (d) => d.id)
      .attr("class", "country")
      .attr("d", this.path)
      .style("fill", (d) => color(d.value))
      .on("click", (event, d) => this.updateSelectedCountries(d));

    // Draw the graticules
    const graticule = d3.geoGraticule();
    d3.select("#graticules")
      .selectAll("path")
      .data([graticule(), graticule.outline()])
      .join("path")
      .attr("d", this.path)
      .attr("fill", "none")
      .attr("stroke", (d, i) => (i ? "black" : "#e0e0e0"));

    // Create the gradient scale
    this.renderGradientScale(color.domain());

    // Generate the country list
    this.createCountryList();
  }

  /**
   * Retrieves and processes medal data for a specific year and medal type.
   * @param {Array} countryData - Array of all country medal data records.
   * @returns {Array} - Array of objects, each containing `country_code` and `totalCount` for the selected year and medal type.
   */
  getSelectedMedalData(countryData) {
    // Filter the data by the selected year, and group by country.
    let yearFilter = countryData;
    if (globalApplicationState.yearSelection != "Total") {
      yearFilter = countryData.filter(
        (d) =>
          parseInt(d.year) === parseInt(globalApplicationState.yearSelection)
      );
    }

    const groupedMedal = d3.group(yearFilter, (d) => d.country);
    let medalData = [];

    // Calculate the total medal count for each country
    groupedMedal.forEach((element) => {
      let totalCount = 0;

      element.forEach((d) => {
        totalCount += +d[globalApplicationState.medalSelection];
      });

      medalData.push({
        country_code: element[0].country_code,
        totalCount: totalCount,
      });
    });

    return medalData;
  }

  /**
   * Renders or updates the gradient scale at the bottom of the map to reflect the color scale domain.
   * @param {Array} domain The [min, max] domain of the color scale.
   */
  renderGradientScale([min, max]) {
    const svg = d3.select("#map");
    const svgHeight = parseInt(window.getComputedStyle(svg.node()).height);
    const gradientVis = svg.select("#gradient");

    // Check if gradient scale elements exist; if not, create them
    if (gradientVis.select("defs #Gradient").empty()) {
      // Create gradient definition
      const gradient = gradientVis
        .append("defs")
        .append("linearGradient")
        .attr("id", "Gradient")
        .attr("x1", "0%")
        .attr("y1", "0%")
        .attr("x2", "100%")
        .attr("y2", "0%");

      // Define gradient color stops
      const colors = ["#fff7ee", "#ffaf96", "#d52220", "#67010f"];
      colors.forEach((color, i) => {
        gradient
          .append("stop")
          .attr("offset", `${(i / (colors.length - 1)) * 100}%`)
          .attr("stop-color", color);
      });

      // Create gradient rectangle
      gradientVis
        .append("rect")
        .attr("x", 0)
        .attr("y", svgHeight - 20)
        .attr("width", 150)
        .attr("height", 50)
        .attr("fill", "url(#Gradient)");

      // Create min and max labels
      gradientVis
        .append("text")
        .attr("id", "min-label")
        .attr("x", 0)
        .attr("y", svgHeight - 25)
        .attr("text-anchor", "left")
        .attr("font-size", "16px");

      gradientVis
        .append("text")
        .attr("id", "max-label")
        .attr("x", 125)
        .attr("y", svgHeight - 25)
        .attr("text-anchor", "center")
        .attr("font-size", "16px");
    }

    // Update min and max labels
    gradientVis.select("#min-label").text(Math.round(min));
    gradientVis.select("#max-label").text(Math.round(max));
  }

  /**
   * Updates the selection and classes of each country, then updates the line chart.
   * @param {Object} selected The selected country data.
   */
  updateSelectedCountries(selected) {
    const { globalApplicationState } = this;
    const selectedLocations = globalApplicationState.selectedLocations;
    const listItem = d3.select(`#list-${selected.id}`);

    if (selectedLocations.some((loc) => loc.id === selected.id)) {
      // Remove selection
      globalApplicationState.selectedLocations = selectedLocations.filter(
        (loc) => loc.id !== selected.id
      );
      globalApplicationState.groupColor =
        globalApplicationState.groupColor.filter((d) => d.id !== selected.id);
      d3.select(`#${selected.id}`).classed("selected", false);
      listItem.classed("selected", false);
    } else {
      // Add selection
      selectedLocations.push(selected);
      d3.select(`#${selected.id}`).classed("selected", true);
      listItem.classed("selected", true);
    }

    if (d3.select("#chartSelect").property("value") === "bar") {
      globalApplicationState.barChart.updateBarChart();
    } else {
      globalApplicationState.lineChart.updateLineChart();
    }
  }

  /**
   * Updates the map visualization based on changes in yearSelection or medalSelection.
   */
  updateMapVisual() {
    const countryData = this.globalApplicationState.medalData;

    // Get updated medal data based on the current year and medal selections
    let medalData = this.getSelectedMedalData(countryData);

    // Update the color scale domain with the new range of values
    const color = d3
      .scaleQuantize()
      .range([
        "#fff7ee",
        "#ffd6ca",
        "#ffaf96",
        "#fb8260",
        "#f44f39",
        "#d52220",
        "#ab1115",
        "#67010f",
      ])
      .domain(d3.extent(medalData, (d) => +d.totalCount));

    // Create a lookup of total medal count for each country
    const dataLookup = d3.rollup(
      medalData,
      (values) => d3.max(values, (d) => +d.totalCount),
      (d) => d.country_code
    );

    // Apply transition to update map colors based on new medal data
    d3.select("#countries")
      .selectAll("path")
      .transition()
      .duration(1000) // Adjust duration as needed
      .style("fill", (d) => color(dataLookup.get(d.id) || 0));

    // Draw the graticules
    const graticule = d3.geoGraticule();
    d3.select("#graticules")
      .selectAll("path")
      .data([graticule(), graticule.outline()])
      .join("path")
      .attr("d", this.path)
      .attr("fill", "none")
      .attr("stroke", (d, i) => (i ? "black" : "#e0e0e0"));

    // Update the gradient scale to reflect the new color scale domain
    this.renderGradientScale(color.domain());

    if (d3.select("#chartSelect").property("value") === "bar") {
      globalApplicationState.barChart.updateBarChart();
    } else {
      globalApplicationState.lineChart.updateLineChart();
    }
  }

  createCountryList() {
    const countryListContainer = d3.select("#country-list");

    // Ensure the container exists
    if (countryListContainer.empty()) {
      console.error(
        "Country list container not found. Make sure the #country-list element exists in the HTML."
      );
      return;
    }

    const countryData = this.globalApplicationState.medalData;

    // Ensure medal data is loaded
    if (!countryData || countryData.length === 0) {
      console.error(
        "Medal data is empty or not loaded. Ensure data is being fetched and parsed correctly."
      );
      return;
    }

    // Get unique country codes and names
    const countries = Array.from(
      d3.group(countryData, (d) => d.country_code),
      ([key, values]) => ({ code: key, name: values[0]?.country || "Unknown" }) // Fallback for undefined country names
    ).sort((a, b) => a.name.localeCompare(b.name)); // Sort alphabetically by name

    // Debug: Log the countries array to ensure it's correct
    console.log("Country list:", countries);

    // Clear any existing content in the list container
    countryListContainer.html("");

    // Add a list of countries to the container
    const ul = countryListContainer.append("ul").attr("class", "country-list");

    ul.selectAll("li")
      .data(countries)
      .join("li")
      .attr("id", (d) => `list-${d.code}`)
      .text((d) => d.name)
      .on("click", (event, d) => {
        console.log("Country clicked:", d); // Debug: Log clicked country
        this.updateSelectedCountries({ id: d.code });
      });
  }
}
