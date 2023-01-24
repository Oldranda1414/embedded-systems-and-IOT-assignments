const lightSwitch = document.getElementById('switch');
const blindRange = document.getElementById('range');
const takeControlSwitch = document.getElementById('control');

let dataLight = [];
const chart = new Chart(document.getElementById('chart'), {
    type: 'line',
    data: {
        labels: [],
        datasets: [{
            label: 'Roller blind',
            data: [],
            fill: false,
            borderColor: 'rgb(53, 215, 233)',
            tension: 0.1
        }, {
            data: [],
            pointRadius: 0,
            segment: {
                borderColor: ctx => dataLight[ctx.p0DataIndex] == 'ON' ? 'rgb(53, 233, 82)' : 'rgb(235, 62, 62)',
            },
            fill: false,
            tension: 0
        }]
    },
    options: {
        plugins: {
            legend: {
                labels: {
                    font: {
                        size: 18
                    },
                    generateLabels: c => [
                        {
                            text: 'Roller blind',
                            fillStyle: 'rgb(53, 215, 233)',
                        }, {
                            text: 'Light ON',
                            fillStyle: 'rgb(53, 233, 82)'
                        }, {
                            text: 'Light OFF',
                            fillStyle: 'rgb(235, 62, 62)'
                        }
                    ]
                }
            },
        },
        responsive: true,
        maintainAspectRatio: false,
        scales: {
            y: {
                suggestedMin: -10,
                suggestedMax: 100
            },
            x: {
                type: 'time',
                time: {
                    unit: 'second'
                }
            }
        }
    }
});


setInterval (() => {
    askForData();
}, 200);

async function askForData() {
    let override = new FormData();
    if (takeControlSwitch.checked) {
        override.append('light', lightSwitch.checked ? 'ON' : 'OFF');
        override.append('rollerBlind', + blindRange.value);
    }
    const resp = await fetch("http://localhost:8088", {
        method: 'POST',
        body: override,
    });
    const data = await resp.json();

    updateChart(data);
}

function updateChart(data) {
    chart.data.labels = Object.keys(data).map(d => new Date(d));
    chart.data.datasets[0].data = Object.values(data).map(o => o.rollerBlind);
    dataLight = Object.values(data).map(o => o.light);
    chart.data.datasets[1].data = new Array(dataLight.length).fill(-10);
    chart.update();
}

function takeControl() {
    lightSwitch.disabled = blindRange.disabled = !takeControlSwitch.checked;
}
