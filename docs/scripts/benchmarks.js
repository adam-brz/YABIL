function convertBenchmarkData(benchmarkData) {
    const benchmarks = benchmarkData.benchmarks;
    const structuredData = {};

    for (const bench of benchmarks) {
        const nameParts = bench.name.split('/');

        if (nameParts.length < 4) continue;

        const [operation, library, sizeStr, metric] = nameParts;
        const size = parseInt(sizeStr);

        if (metric.endsWith('_mean') || metric.endsWith('_stddev')) {
            structuredData[operation] ??= {};
            structuredData[operation][library] ??= {};
            structuredData[operation][library][size] ??= { mean: null, stddev: null };

            if (metric.endsWith('_mean')) {
                structuredData[operation][library][size].mean = bench.real_time;
            } else if (metric.endsWith('_stddev')) {
                structuredData[operation][library][size].stddev = bench.real_time;
            }
        }
    }

    return structuredData;
}

function generatePlots(structuredData, container_name) {
    const container = document.getElementById(container_name);

    for (const [operation, libraries] of Object.entries(structuredData)) {
        const chartDiv = document.createElement('div');
        chartDiv.id = `chart-${operation}`;
        chartDiv.style.marginBottom = '50px';
        container.appendChild(chartDiv);

        const traces = [];

        for (const [library, sizeData] of Object.entries(libraries)) {
            const x = [];
            const y = [];
            const errorY = [];

            for (const [size, values] of Object.entries(sizeData)) {
                if (values.mean != null && values.stddev != null) {
                    x.push(Number(size));
                    y.push(values.mean);
                    errorY.push(values.stddev);
                }
            }

            // Sort by size ascending
            const zipped = x.map((_, i) => ({ size: x[i], mean: y[i], stddev: errorY[i] }));
            zipped.sort((a, b) => a.size - b.size);

            const sortedX = zipped.map(d => d.size);
            const sortedY = zipped.map(d => d.mean);
            const sortedErrorY = zipped.map(d => d.stddev);

            traces.push({
                name: library,
                x: sortedX,
                y: sortedY,
                error_y: {
                    type: 'data',
                    array: sortedErrorY,
                    visible: true
                },
                type: 'scatter',
                mode: 'lines+markers'
            });
        }

        const layout = {
            title: { text: `${operation} Benchmark` },
            margin: { t: 50 },
            paper_bgcolor: "rgb(255,255,255)", 
            plot_bgcolor: "rgb(229,229,229)", 
            xaxis: { title: { text: 'Number size in digits' } },
            yaxis: { title: { text: 'Time (ns)' } },
            autosize: true,
            responsive: true,
        };

        Plotly.newPlot(chartDiv.id, traces, layout);
    }
}

function draw_from_benchmarks(benchmark_file, container_name) {
    fetch(benchmark_file)
        .then(response => response.json())
        .then(benchmarkData => {
            const structuredData = convertBenchmarkData(benchmarkData);
            generatePlots(structuredData, container_name);
        })
        .catch(error => {
            console.error('Error loading data:', error);
        });
}