/** Class representing the line chart view. */
class LineChart {
  /**
   * Creates a LineChart
   * @param globalApplicationState The shared global application state (has the data and map instance in it)
   */
  constructor(globalApplicationState) {
    this.globalApplicationState = globalApplicationState;
    let countryData = globalApplicationState.medalData;

    const topCountry = this.getTopFive();
    const filteredCountry = countryData.filter(d => topCountry.includes(d.country_code));
    const grouped = d3.group(filteredCountry, d => d.country_code);
    
    this.initChart();
    this.drawLineChart(filteredCountry, grouped);
  }

  initChart() {
    const svg = d3.select("#line-chart")
    const svgHeight = parseInt(window.getComputedStyle(svg.node()).height);
    const svgWidth = parseInt(window.getComputedStyle(svg.node()).width) * 0.9;

    // Initialize x-axis scale
    this.xScale = d3.scaleTime()
    .domain([new Date(2008, 0, 1), new Date(2024, 0, 1)])
    .range([80, svgWidth - 15])
    .nice();

    const specificYears = [2008, 2012, 2016, 2020, 2024];
    const tickDates = specificYears.map(year => new Date(year, 0, 1));

    // Draw x-axis with custom ticks
    svg.select("#x-axis")
    .attr("transform", `translate(0,${svgHeight - 50})`)
    .call(d3.axisBottom(this.xScale)
      .tickValues(tickDates) 
      .tickFormat(d3.timeFormat("%Y"))); 

    // Add axis labels
    const g = svg.append("g").attr("id", "labels");
    g.append("text")
      .attr("id", "x-label")
      .attr("x", svgWidth / 2 + 50)
      .attr("y", svgHeight - 10)
      .attr("text-anchor", "middle")
      .text("Years");

    g.append("text")
      .attr("id", "y-label")
      .attr("x", -svgHeight / 2)
      .attr("y", 20)
      .attr("transform", "rotate(-90)")
      .attr("text-anchor", "middle")
      .text(`${globalApplicationState.medalSelection} Medals`);
  }

  /**
   * Draws the line chart using the selected data and the data grouped by location or continent.
   * @param {*} data 
   * @param {*} groupedData 
   */
  drawLineChart(data, groupedData) {
    const svg = d3.select("#line-chart");
    const svgHeight = parseInt(window.getComputedStyle(svg.node()).height);
    const svgWidth = parseInt(window.getComputedStyle(svg.node()).width) - 15;

    // Initialize y-axis scale
    const maxCases = d3.max(data, d => +d[this.globalApplicationState.medalSelection]);
    const yScale = d3.scaleLinear()
      .domain([0, maxCases])
      .range([svgHeight - 50, 50])
      .nice();

    // Draw y-axis
    d3.select("#y-label").text(`${this.globalApplicationState.medalSelection} Medals`);
    d3.select("#y-axis")
      .attr("transform", "translate(80,0)")
      .call(d3.axisLeft(yScale));

    // Dark color generation function
    const generateDarkColor = () => {
      const r = Math.floor(Math.random() * 180);
      const g = Math.floor(Math.random() * 180);
      const b = Math.floor(Math.random() * 180);
      return `rgb(${r}, ${g}, ${b})`;
    };

    // Ensure lines start with a clean slate
    const linesGroup = svg.select("#lines");
    linesGroup.selectAll("path").remove();

    // Generate line path
    const lineGenerator = d3.line()
      .x(d => this.xScale(new Date(d.year, 0, 1)))
      .y(d => yScale(+d[this.globalApplicationState.medalSelection]));

    const groupColorMap = new Map(this.globalApplicationState.groupColor.map(d => [d.country_code, d.color]));
    
    groupedData.forEach((values, location) => {
      const country_code = values[0]?.country_code;
      let colorEntry = groupColorMap.get(country_code);
    
      if (!colorEntry && country_code) {
        colorEntry = generateDarkColor();
        this.globalApplicationState.groupColor.push({ country_code: country_code, color: colorEntry });
        groupColorMap.set(country_code, colorEntry);
      }
    
      if (values.length > 0) {
        const path = linesGroup.append("path")
          .datum(values)
          .attr("d", lineGenerator)
          .attr("fill", "none")
          .attr("stroke", colorEntry)
          .attr("stroke-width", 2);

        // Get total length of the path
        const totalLength = path.node().getTotalLength();

        // Apply animation
        path
          .attr("stroke-dasharray", totalLength) // Set up the dasharray to match path length
          .attr("stroke-dashoffset", totalLength) // Offset to the start of the path
          .transition()
          .duration(500) // Duration of the animation
          .ease(d3.easeLinear) // Ease function for smoothness
          .attr("stroke-dashoffset", 0); // Animate the stroke-dashoffset to 0
      }
    });

    this.initMouseInteractions(svgWidth, svgHeight, data);
  }

  initMouseInteractions(svgWidth, svgHeight, data) {
    const svg = d3.select("#line-chart");
    const overlay = svg.select("#overlay").html(""); // Clear previous overlay elements

    overlay.append("line")
      .attr("id", "mouse_line")
      .attr("y1", 50)
      .attr("y2", svgHeight - 50)
      .attr("stroke", "black")
      .attr("opacity", "0");

    const textGroup = overlay.append("g").attr("id", "text");

    svg.on("mousemove", event => this.handleMouseMove(event, svgWidth, textGroup, data));
  }

  handleMouseMove(event, svgWidth, textGroup, data) {
    const svg = d3.select("#line-chart");
    let [x] = d3.pointer(event);
    x = Math.max(80, Math.min(x, svgWidth));

    const year = this.formatDate(new Date(this.xScale.invert(x)));
    
    const filteredData = data
      .filter(d => +d.year === +year)
      .sort((a, b) => b[globalApplicationState.medalSelection] - a[globalApplicationState.medalSelection]);
     
    const xPosition = x < 230 ? x + 10 : x - 150;
    let yPosition = 50;

    textGroup.selectAll("text").remove();

    textGroup.append("text")
    .text(`Year: ${year}`)
    .attr("x", xPosition)
    .attr("y", yPosition)
    .attr("fill", "black");

    yPosition += 20;

    filteredData.forEach(d => {
      const color = this.globalApplicationState.groupColor.find(c => c.country_code === d.country_code)?.color || "black";
      textGroup.append("text")
        .text(`${d.country} ${this.formatValue(+d[globalApplicationState.medalSelection])}`)
        .attr("x", xPosition)
        .attr("y", yPosition)
        .attr("fill", color);

      yPosition += 20;
    });

    svg.select("#mouse_line")
      .attr("opacity", "1")
      .attr("x1", x)
      .attr("x2", x);
  }

  formatValue(value) {
    return value < 500 ? d3.format(",")(parseInt(value)) : d3.format(",.3s")(value);
  }

  formatDate(date) {
    const year = date.getFullYear();
    if (year >= 2008 && year < 2012 || year < 2008) {
      return 2008;
    } else if (year >= 2012 && year < 2016) {
      return 2012;
    } else if (year >= 2016 && year < 2020) {
      return 2016;
    } else if (year >= 2020 && year < 2024) {
      return 2020;
    }else if (year >= 2024) {
      return 2024;
    }
  }

  /**
     * Retrieves the top 5 countries by total medals
   */
  getTopFive(){
    const groupedMedal = d3.group(globalApplicationState.medalData, d => d.country_code);
    let medalData = [];

    // Calculate the total medal count for each country
    groupedMedal.forEach(element => {
      let totalCount = 0;

      element.forEach(d => {
        totalCount += +d[globalApplicationState.medalSelection];
      });

      medalData.push({ country_code: element[0].country_code, totalCount: totalCount });
    });

    medalData.sort((a,b)=> b.Total - a.Total);
    const topFive = medalData.slice(0, 5);

    return [...new Set(topFive.map(d => d.country_code))];
  }

  /**
   * Updates the line chart based on the selected countries
   */
  updateLineChart() {    
    const selectedLocations = this.globalApplicationState.selectedLocations;

    const flatData = selectedLocations.length === 0
      ? this.globalApplicationState.medalData.filter(d => this.getTopFive().includes(d.country_code))
      : selectedLocations.flatMap(loc => 
          this.globalApplicationState.medalData.filter(d => d.country_code.includes(loc.id))
        );

    const groupedData = d3.group(flatData, d => d.country_code);
    this.drawLineChart(flatData, groupedData);
  }
}