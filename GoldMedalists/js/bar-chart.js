class BarChart {
    /**
     * Creates a BarChart
     * @param globalApplicationState The shared global application state (has the data and map instance in it)
     */
    constructor(globalApplicationState) {
        this.globalApplicationState = globalApplicationState;
        let countryData = globalApplicationState.medalData;

        const topCountry = this.getTopFive();
        const filteredCountry = countryData.filter(d => topCountry.includes(d.country_code));
    
        this.initChart();
        this.renderChart(filteredCountry);
    }

    /**
     * Initializes the SVG, scales, and axes for the bar chart.
     */
    initChart() {
        const svg = d3.select("#bar-chart");
        const svgWidth = parseInt(window.getComputedStyle(svg.node()).width) * 0.9;
        const svgHeight = parseInt(window.getComputedStyle(svg.node()).height);

        this.xScale = d3.scaleLinear().range([0, svgWidth - 100]);
        this.yScale = d3.scaleBand().range([0, svgHeight - 50]).padding(0.1);


        const { yearSelection, medalSelection, medalData, selectedLocations } = this.globalApplicationState;
        // Filter data by the selected year if it's not "Total"
        let filteredData = medalData;
        if (yearSelection !== "Total") {
            filteredData = medalData.filter(d => parseInt(d.year) === parseInt(yearSelection));
        }
        const yearLabel = yearSelection === "Total" ? "All Years" : yearSelection;
        const medalLabel = medalSelection === "Total" ? "Total Medals" : `${medalSelection} Medals`;

        svg.append("g")
            .attr("id", "x-axis")
            .attr("transform", `translate(75, ${svgHeight - 40})`);

        svg.append("g")
            .attr("id", "y-axis")
            .attr("transform", "translate(75,0)");

        svg.append("text")
            .attr("id", "x-label-bar-chart")
            .attr("x", svgWidth / 2)
            .attr("y", svgHeight - 5)
            .attr("text-anchor", "middle")
            .text(`${medalLabel} in ${yearLabel}`);

        svg.append("text")
            .attr("id", "y-label-bar-chart")
            .attr("x", -svgHeight / 2)
            .attr("y", 20)
            .attr("transform", "rotate(-90)")
            .attr("text-anchor", "middle")
            .text("Country");
    }

    /**
     * Renders the bar chart with the provided data.
     * @param {Array} dataToDisplay - Array of objects containing country_code and Total.
     */
    renderChart(dataToDisplay) {
        const svg = d3.select("#bar-chart");
        const svgHeight = parseInt(svg.style("height"));


        const { yearSelection, medalSelection, medalData, selectedLocations } = this.globalApplicationState;
        // Filter data by the selected year if it's not "Total"
        let filteredData = medalData;
        if (yearSelection !== "Total") {
            filteredData = medalData.filter(d => parseInt(d.year) === parseInt(yearSelection));
        }

        // Dynamically update the x-axis label
        const yearLabel = yearSelection === "Total" ? "All Years" : yearSelection;
        const medalLabel = medalSelection === "Total" ? "Total Medals" : `${medalSelection} Medals`;
        d3.select("#x-label-bar-chart").text(`${medalLabel} in ${yearLabel}`);
       
        // Sort data in descending order and set up scales
        dataToDisplay.sort((a, b) => b.Total - a.Total);
        this.xScale.domain([0, d3.max(dataToDisplay, d => d.Total)]);
        this.yScale.domain(dataToDisplay.map(d => d.country_code));

        // Update x-axis
        svg.select("#x-axis")
            .call(d3.axisBottom(this.xScale).ticks(5).tickFormat(d3.format(",.0f")))
            .attr("transform", `translate(75, ${svgHeight - 40})`);

        // Update y-axis
        svg.select("#y-axis")
            .call(d3.axisLeft(this.yScale).tickSize(0).tickPadding(10))
            .attr("transform", "translate(75, 0)");

        // Bind data to bars
        const bars = svg.selectAll(".bar")
            .data(dataToDisplay, d => d.country_code);

        bars.exit().remove();

        bars.transition()
            .duration(500)
            .attr("x", 75)
            .attr("y", d => this.yScale(d.country_code))
            .attr("width", d => this.xScale(d.Total))
            .attr("height", this.yScale.bandwidth())
            .style("fill", "steelblue");

        bars.enter().append("rect")
            .attr("class", "bar")
            .attr("x", 75)
            .attr("y", d => this.yScale(d.country_code))
            .attr("width", d => this.xScale(d.Total))
            .attr("height", this.yScale.bandwidth())
            .style("fill", "steelblue");

        // Add labels with total medals
        const labels = svg.selectAll(".label")
            .data(dataToDisplay, d => d.country_code);

        labels.exit().remove();

        labels.transition()
            .duration(1000)
            .attr("x", d => this.xScale(d.Total) + 77)
            .attr("y", d => this.yScale(d.country_code) + this.yScale.bandwidth() / 2)
            .attr("dy", ".35em")
            .attr("text-anchor", "start")
            .text(d => d.Total)
            .style("font-size", "14px")
            .style("fill", "black");

        labels.enter().append("text")
            .attr("class", "label")
            .attr("x", d => this.xScale(d.Total) + 77)
            .attr("y", d => this.yScale(d.country_code) + this.yScale.bandwidth() / 2)
            .attr("dy", ".35em")
            .attr("text-anchor", "start")
            .text(d => d.Total)
            .style("font-size", "14px")
            .style("fill", "black");
    }

    /**
     * Retrieves the top 5 countries by total medals
     */
    getTopFive() {
        const groupedMedal = d3.group(globalApplicationState.medalData, d => d.country_code);
        let medalData = this.getGroupTotal(groupedMedal);
    
        medalData.sort((a,b)=> b.Total - a.Total);
        const topFive = medalData.slice(0, 5);
    
        return [...new Set(topFive.map(d => d.country_code))];
    }

    /**
     * Returns an array of countries with a singular total value
     */
    getGroupTotal(groupedMedal){
        let medalData = [];
    
        // Calculate the total medal count for each country
        groupedMedal.forEach(element => {
          let totalCount = 0;
    
          element.forEach(d => {
            totalCount += +d[globalApplicationState.medalSelection];
          });
    
          medalData.push({ country_code: element[0].country_code, Total: totalCount });
        });

        return medalData;
    }

    /**
     * Updates the bar chart based on selected countries or the top 5 countries.
     */
    updateBarChart() {
        const { yearSelection, medalSelection, medalData, selectedLocations } = this.globalApplicationState;

        // Filter data by the selected year if it's not "Total"
        let filteredData = medalData;
        if (yearSelection !== "Total") {
            filteredData = medalData.filter(d => parseInt(d.year) === parseInt(yearSelection));
        }

        // Determine data to display based on selected locations or top 5 countries
        const flatData = selectedLocations.length === 0
            ? filteredData.filter(d => this.getTopFive().includes(d.country_code))
            : selectedLocations.flatMap(loc => 
                filteredData.filter(d => d.country_code.includes(loc.id))
            );

        // Group and render the filtered data
        const groupedData = d3.group(flatData, d => d.country_code);
        this.renderChart(this.getGroupTotal(groupedData));

        // Dynamically update the x-axis label
        const yearLabel = yearSelection === "Total" ? "All Years" : yearSelection;
        const medalLabel = medalSelection === "Total" ? "Total Medals" : `${medalSelection} Medals`;
        d3.select("#x-label-bar-chart").text(`${medalLabel} in ${yearLabel}`);
    }
}
    

