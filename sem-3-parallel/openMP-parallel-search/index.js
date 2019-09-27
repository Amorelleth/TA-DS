const showFile = () => {
    const firstChart = document.getElementById("chart-search");
    const thirdChart = document.getElementById("chart-parallel-search-change-threads");

    const file = document.querySelector('input[type=file]').files[0];
    const reader = new FileReader();
    const textFile = /text.*/;

    if (file.type.match(textFile)) {
        reader.onload = event => {
            const search = [];
            const parallelSearchCheckSize = [];
            const parallelSearchCheckThreads = [];

            event.target.result
                .split("\n")
                .forEach(line => {
                    const item = line.split(" ");
                    const convertedItem = item.slice(1).map(i => parseFloat(i, 10));
                    if (item[0] === "0") search.push(convertedItem);
                    else if (item[0] === "1") parallelSearchCheckSize.push(convertedItem);
                    else if (item[0] === "2") parallelSearchCheckThreads.push(convertedItem.slice(1).reverse());
                });


            google.charts.load('current', { packages: ['corechart', 'line']});
            google.charts.setOnLoadCallback(() => {
                const defaultOptions = {
                    hAxis: { title: 'Size' },
                    vAxis: { title: 'Time' },
                    legend: { position: 'bottom' },
                    height: 800,
                    width: 800,
                    explorer: { actions: ['dragToZoom', 'rightClickToReset'] }
                };

                const data = new google.visualization.DataTable();
                const chart = new google.visualization.LineChart(firstChart);
                data.addColumn('number', 'X');
                data.addColumn('number', 'linear random');
                data.addColumn('number', 'linear best');
                data.addColumn('number', 'linear worst');
                data.addColumn('number', 'parallel (size) random');
                data.addColumn('number', 'parallel (size) best');
                data.addColumn('number', 'parallel (size) worst');
                data.addRows(search.map((i, idx) => [...i, ...parallelSearchCheckSize[idx].slice(1)]));
                chart.draw(data, { ...defaultOptions, title: 'Linear Simple Search' });
                firstChart.classList.remove("hidden");

                const data2 = new google.visualization.DataTable();
                const chart2 = new google.visualization.LineChart(thirdChart);
                data2.addColumn('number', 'X');
                data2.addColumn('number', 'parallel (threads) worst');
                data2.addColumn('number', 'parallel (threads) best');
                data2.addColumn('number', 'parallel (threads) random');
                data2.addRows(parallelSearchCheckThreads);
                chart2.draw(data2, google.charts.Line.convertOptions({
                    ...defaultOptions,
                    title: 'Parallel Search (Threads)',
                    hAxis: { title: 'Threads' },
                    vAxis: { title: 'Time' },
                }));
                thirdChart.classList.remove("hidden");
            });
        }
    }

    reader.readAsText(file);
}

window.onload = () => {
    if (!window.File || !window.FileReader || !window.FileList || !window.Blob) {
        alert("Your browser is too old to support HTML5 File API");
    }
}
